Entity, Component

Entity represent things in the game world
Components attach properties to the entity

Add Entity: creates a new entity with no properties
Add Component: adds a new component to an entity, adding its properties
Remove Component: removes a component from an entity
Remove Entity: removes entity and all its attached components

```C
enum ComponentType;

#define MAX_NUM_ENTIY_COMPONENTS 128
typedef struct Entity {
    EntityID id;
    struct {
        ComponentType type[MAX_NUM_ENTIY_COMPONENTS];
        ComponentID id[MAX_NUM_ENTIY_COMPONENTS];
    } components;
} Entity;

#define MAX_NUM_ENTITES 128
typedef struct EntityManager {
    Entity entities[MAX_NUM_ENTITES];
} EntityManager;

EntityID EntityManager_add_entity(EntityManager* entities, void);
void EntityManager_remove_entity(EntityManager* entities, EntityID id);

void EntityManager_add_position(EntityManager* entities, EntityID id, Vector2 position);
void EntityManager_get_position(EntityManager* entities, EntityID id, Vector2* position);
void EntityManager_set_position(EntityManager* entities, EntityID id, Vector2 position);
bool EntityManager_has_position(EntityManager* entities, EntityID id);

void EntityManager_add_sprite(EntityManager* entities, EntityID id, Sprite sprite);
void EntityManager_get_sprite(EntityManager* entities, EntityID id, Sprite* sprite);
void EntityManager_set_sprite(EntityManager* entities, EntityID id, Sprite sprite);
bool EntityManager_has_sprite(EntityManager* entities, EntityID id);
```
