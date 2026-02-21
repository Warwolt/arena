// Pool for storing ECS components
//
// Usage:
// - Define a Pool struct for your component
// - Use these macros to simplify usage
//
// Interface:
// 		struct Pool<T,N> {
// 	        int keys[N];
// 	        T values[N];
// 	        int size;
// 		}
//
// Example:
//      #define MAX_POSITIONS (int)128
//      typedef struct PositionPool {
// 	        int keys[MAX_POSITIONS];
// 	        Vector2 values[MAX_POSITIONS];
// 	        int size;
//      } PositionPool;
//
//      PositionPool pool = { 0 };
//      Pool_add_item(&pool, entity_id, (Vector2) { 12, 34 });

#pragma once

// pool: Pool<T>*
// id: int
// item: T
#define Pool_add_item(pool, id, item)        \
	do {                                     \
		(pool)->keys[(pool)->size] = id;     \
		(pool)->values[(pool)->size] = item; \
		(pool)->size++;                      \
	} while (0)

// pool: Pool<T>*
// id: int
#define Pool_remove_item(pool, id)                                    \
	do {                                                              \
		size_t i = 0;                                                 \
		while ((pool)->keys[i] != 0) {                                \
			if ((pool)->keys[i] == id) {                              \
				(pool)->keys[i] = (pool)->keys[(pool)->size - 1];     \
				(pool)->values[i] = (pool)->values[(pool)->size - 1]; \
				(pool)->size--;                                       \
				break;                                                \
			}                                                         \
			i++;                                                      \
		}                                                             \
	} while (0)

// pool: Pool<T>*
// id: int
// item: T*
#define Pool_get_item(pool, id, item)        \
	do {                                     \
		size_t i = 0;                        \
		while ((pool)->keys[i] != 0) {       \
			if ((pool)->keys[i] == id) {     \
				*(item) = (pool)->values[i]; \
				break;                       \
			}                                \
			i++;                             \
		}                                    \
	} while (0)

// pool: Pool<T>*
// id: int
// item: T
#define Pool_set_item(pool, id, item)       \
	do {                                    \
		size_t i = 0;                       \
		while ((pool)->keys[i] != 0) {      \
			if ((pool)->keys[i] == id) {    \
				(pool)->values[i] = (item); \
				break;                      \
			}                               \
			i++;                            \
		}                                   \
	} while (0)
