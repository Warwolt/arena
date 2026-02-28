#include <rktest/rktest.h>

#include "engine/entity.h"

#include <raymath.h>

TEST(EntityTests, AddEntity_ReturnsID) {
	EntityManager entities = { 0 };

	EntityID id1 = EntityManager_add_entity(&entities, Vector2Zero());
	EntityID id2 = EntityManager_add_entity(&entities, Vector2Zero());
	EntityID id3 = EntityManager_add_entity(&entities, Vector2Zero());

	EXPECT_EQ((int)id1.value, 1);
	EXPECT_EQ((int)id2.value, 2);
	EXPECT_EQ((int)id3.value, 3);
	EXPECT_EQ((int)entities.entities.size, 3);
}

TEST(EntityTests, AddEntity_AddComponent_AttachesComponentToEntity) {
	EntityManager entities = { 0 };

	EntityID id = EntityManager_add_entity(&entities, Vector2Zero());
	EntityManager_add_collision_shape(&entities, id, Shape_circle((Circle) { 0 }));
	EntityManager_add_sprite(&entities, id, (Sprite) { (TextureID) { 1 }, (Rectangle) { 0 } });

	EXPECT_EQ((int)entities.entities.size, 1);
	EXPECT_EQ((int)entities.entities.values[0].num_components, 2);
	EXPECT_EQ(entities.entities.values[0].components[0], ComponentType_CollisionShape);
	EXPECT_EQ(entities.entities.values[0].components[1], ComponentType_Sprite);
	EXPECT_EQ((int)entities.components.collision_shapes.size, 1);
	EXPECT_EQ((int)entities.components.sprites.size, 1);
}

TEST(EntityTests, AddEntity_RemoveEntity_ComponentsAlsoRemoved) {
	EntityManager entities = { 0 };

	EntityID id = EntityManager_add_entity(&entities, Vector2Zero());
	EntityManager_add_collision_shape(&entities, id, Shape_circle((Circle) { 0 }));
	EntityManager_remove_entity(&entities, id);

	EXPECT_EQ((int)entities.entities.size, 0);
	EXPECT_EQ((int)entities.components.collision_shapes.size, 0);
}

TEST(EntityTests, AddEntity_RemoveEntity_AddEntity_ReusesID) {
	EntityManager entities = { 0 };

	EntityID id1 = EntityManager_add_entity(&entities, Vector2Zero());
	EntityID id2 = EntityManager_add_entity(&entities, Vector2Zero());
	EntityManager_remove_entity(&entities, id1);
	EntityID id3 = EntityManager_add_entity(&entities, Vector2Zero());

	EXPECT_EQ((int)id1.value, 1);
	EXPECT_EQ((int)id2.value, 2);
	EXPECT_EQ((int)id3.value, 1);
}
