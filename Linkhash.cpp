#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>
#include<unordered_map> // hash map

int BUCKETSIZE[10] = { 13, 29, 53, 97, 193, 389, 769, 1543, 3079, 6151 };
#define BUCKETSIZE_COUNT (sizeof(BUCKETSIZE)/sizeof(BUCKETSIZE[0]))
typedef int KeyType;
typedef struct {} Record;
typedef struct {
	KeyType key;
	Record* info;
}ElemType;
typedef struct HashNode {
	ElemType data;
	struct HashNode* next;
} HashNode;
typedef struct {
	HashNode** map;
	size_t bucket_size;
	size_t current_size;
    size_t bucket_index; /* per-table index into BUCKETSIZE */
}HashTable;

void InitHash(HashTable* pt) {
	assert(pt != NULL);
    pt->current_size = 0;
	pt->bucket_index = 0;
	pt->bucket_size = BUCKETSIZE[pt->bucket_index];
	pt->map = (HashNode**)calloc(pt->bucket_size, sizeof(HashNode*));
	if (pt->map == NULL) {
		perror("calloc failed");
		exit(EXIT_FAILURE);
	}
}

int HashFunc(HashTable* pt, KeyType key) {
    assert(pt != NULL);
	if (pt->map == NULL || pt->bucket_size == 0) {
		// Lazily initialize if table was freed or never initialized
		InitHash(pt);
	}
	size_t k = (size_t)key;
	return (int)(k % pt->bucket_size);
}

HashNode* CreateNode(ElemType data) {
	HashNode* node = (HashNode*)malloc(sizeof(HashNode));
	if(node == NULL) {
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	node->data = data;
	node->next = NULL;
	return node;
}

HashNode* FindNode(HashTable* pt, KeyType key) {
    assert(pt != NULL);
	if (pt->map == NULL || pt->bucket_size == 0) InitHash(pt);
	int index = HashFunc(pt, key);
	HashNode* node = pt->map[index];
	while(node != NULL) {
		if(node->data.key == key) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}

bool InsertHash(HashTable* pt, ElemType data) {
    assert(pt != NULL);
	if (pt->map == NULL || pt->bucket_size == 0) InitHash(pt);

	if(FindNode(pt, data.key) != NULL) {
		return false; // already exists
	}
    int index = HashFunc(pt,data.key);
	HashNode* node = CreateNode(data);
	node->next = pt->map[index];
	pt->map[index] = node;
	pt->current_size++;
	return true;
}

void PrintHash(HashTable* pt) {
    if (pt == NULL || pt->map == NULL) return;
	for(size_t i = 0; i < pt->bucket_size; i++) {
		HashNode* node = pt->map[i];
		printf("Bucket %2zu: ", i);
		while(node != NULL) {
			printf("Key:%d ", node->data.key);
			node = node->next;
		}
		printf("\n");
	}
}

void FreeHash(HashTable* pt) {
    if (pt == NULL || pt->map == NULL) return;
	for(size_t i = 0; i < pt->bucket_size; i++) {
		HashNode* node = pt->map[i];
		while(node != NULL) {
			HashNode* temp = node;
			node = node->next;
			free(temp);
		}
	}
	free(pt->map);
	pt->map = NULL;
	pt->current_size = 0;
	pt->bucket_size = 0;
}

bool RemoveHash(HashTable* pt, KeyType key) {
    assert(pt != NULL);
	if (pt->map == NULL || pt->bucket_size == 0) InitHash(pt);
	int index = HashFunc(pt,key);
	HashNode* node = pt->map[index];
	HashNode* prev = NULL;
	while(node != NULL) {
		if(node->data.key == key) {
			if(prev == NULL) {
				pt->map[index] = node->next;
			} else {
				prev->next = node->next;
			}
			free(node);
			pt->current_size--;
			return true;
		}
		prev = node;
		node = node->next;
	}
	return false; // not found
}

// Hash扩容
void ResizeHash(HashTable* pt) {
    assert(pt != NULL);
	if (pt->map == NULL || pt->bucket_size == 0) InitHash(pt);
	if (pt->bucket_index + 1 >= BUCKETSIZE_COUNT) {
		// cannot resize beyond predefined sizes
		return;
	}
	pt->bucket_index++;
	size_t new_bucket_size = BUCKETSIZE[pt->bucket_index];
	HashNode** new_map = (HashNode**)calloc(new_bucket_size, sizeof(HashNode*));
	if(new_map == NULL) {
		perror("calloc failed");
		exit(EXIT_FAILURE);
	}
	// Rehash all existing nodes
	for(size_t i = 0; i < pt->bucket_size; i++) {
		HashNode* node = pt->map[i];
		while(node != NULL) {
			HashNode* next_node = node->next; // save next node
            size_t new_index = ((size_t)node->data.key) % new_bucket_size;
			node->next = new_map[new_index];
			new_map[new_index] = node;
			node = next_node;
		}
	}
	free(pt->map);
	pt->map = new_map;
	pt->bucket_size = new_bucket_size;
}
#if 0
int main() {
	HashTable ht;
	InitHash(&ht);
	ElemType e1 = {18, NULL};
	ElemType e2 = {14, NULL};
	ElemType e3 = {26, NULL};
	ElemType e4 = { 26, NULL };
	InsertHash(&ht, e1);
	InsertHash(&ht, e2);
	InsertHash(&ht, e3);
	InsertHash(&ht, e4); // duplicate key, should not be inserted 
	PrintHash(&ht);
	printf("\nRemoving key 14...\n\n");
	RemoveHash(&ht, 14);
	PrintHash(&ht);
	FreeHash(&ht);
	// Test resizing
	for (int i = 0; i < 100; i++) {
		ElemType e = {i, NULL};
		InsertHash(&ht, e);
        /* Trigger resize when current_size reaches the load factor threshold.
		   Use integer math to avoid floating-point rounding issues: current_size >= ceil(bucket_size * 0.75)
		   Equivalent check: current_size * 4 >= bucket_size * 3 */
		if (ht.current_size * 4 >= ht.bucket_size * 3) {
			printf("\nResizing hash table...\n\n");
			ResizeHash(&ht);
			PrintHash(&ht);
		}
	}
	return 0;
}
#endif