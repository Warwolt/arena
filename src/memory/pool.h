// Pool for storing ECS components
//
// Usage:
// - Define a Pool struct for your component
// - Use these macros to simplify usage
//
// Interface:
// 		struct Pool<T,N> {
// 	        EntityID keys[N];
// 	        T values[N];
// 	        int size;
// 		}
//
// Example:
//      #define MAX_POSITIONS (int)128
//      typedef struct PositionPool {
// 	        EntityID keys[MAX_POSITIONS];
// 	        Vector2 values[MAX_POSITIONS];
// 	        int size;
//      } PositionPool;
//
//      PositionPool pool = { 0 };
//      Pool_add_item(&pool, entity_id, (Vector2) { 12, 34 });

#pragma once

#include "entity.h"

// pool: Pool<T>*
// id: EntityID
// item: T
#define Pool_add_item(pool, id, item)        \
	do {                                     \
		(pool)->keys[(pool)->size] = id;     \
		(pool)->values[(pool)->size] = item; \
		(pool)->size++;                      \
	} while (0)

// pool: Pool<T>*
// id: EntityID
#define Pool_remove_item(pool, id)                                              \
	do {                                                                        \
		size_t _pool_index = 0;                                                 \
		while ((pool)->keys[_pool_index].value != 0) {                          \
			if ((pool)->keys[_pool_index].value == id.value) {                  \
				(pool)->keys[_pool_index] = (pool)->keys[(pool)->size - 1];     \
				(pool)->values[_pool_index] = (pool)->values[(pool)->size - 1]; \
				(pool)->size--;                                                 \
				break;                                                          \
			}                                                                   \
			_pool_index++;                                                      \
		}                                                                       \
	} while (0)

// pool: Pool<T>*
// id: EntityID
// item: T*
#define Pool_get_item(pool, id, item)                          \
	do {                                                       \
		size_t _pool_index = 0;                                \
		while ((pool)->keys[_pool_index].value != 0) {         \
			if ((pool)->keys[_pool_index].value == id.value) { \
				*(item) = (pool)->values[_pool_index];         \
				break;                                         \
			}                                                  \
			_pool_index++;                                     \
		}                                                      \
	} while (0)

// pool: Pool<T>*
// id: EntityID
// item: T
#define Pool_set_item(pool, id, item)                          \
	do {                                                       \
		size_t _pool_index = 0;                                \
		while ((pool)->keys[_pool_index].value != 0) {         \
			if ((pool)->keys[_pool_index].value == id.value) { \
				(pool)->values[_pool_index] = (item);          \
				break;                                         \
			}                                                  \
			_pool_index++;                                     \
		}                                                      \
	} while (0)
