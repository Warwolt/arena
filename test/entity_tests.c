#include <rktest/rktest.h>

#include "entity.h"

TEST(EntityTests, AddEntity_ReturnsID) {
	EntityManager entities = { 0 };

	EntityID id1 = EntityManager_add_entity(&entities);
	EntityID id2 = EntityManager_add_entity(&entities);
	EntityID id3 = EntityManager_add_entity(&entities);

	EXPECT_EQ((int)id1.value, 1);
	EXPECT_EQ((int)id2.value, 2);
	EXPECT_EQ((int)id3.value, 3);
	EXPECT_EQ((int)entities.entities.size, 3);
}

TEST(EntityTests, AddEntity_AddComponent_AttachesComponentToEntity) {
	EntityManager entities = { 0 };

	EntityID id = EntityManager_add_entity(&entities);
	EntityManager_add_position(&entities, id, (Vector2) { 1, 2 });
	EntityManager_add_sprite(&entities, id, (Sprite) { (TextureID) { 1 }, (Rectangle) { 0 } });

	EXPECT_EQ((int)entities.entities.size, 1);
	EXPECT_EQ((int)entities.entities.values[0].num_components, 2);
	EXPECT_EQ(entities.entities.values[0].components[0], ComponentType_Position);
	EXPECT_EQ(entities.entities.values[0].components[1], ComponentType_Sprite);
}

// add entity, add component, remove entity, component removed
