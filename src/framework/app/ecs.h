#pragma once

#include <stdint.h>
#include <rfklib/log.h>

#define ECS_MAX_ENTITIES 1024
#define ECS_MAX_COMPONENTS 64 // uint64 limitation
#define ECS_MAX_SYSTEMS 256

typedef uint64_t ComponentMask;
typedef uint32_t EntityID;

typedef enum : uch {
  SYSTEM_CATEGORY_UPDATE,
  SYSTEM_CATEGORY_RENDER,
  SYSTEM_CATEGORY_PHYSICS,
  SYSTEM_MAX_CATEGORIES
} SystemCategory;

struct ECSContext;
typedef void (*SystemUpdateFunc)(struct ECSContext* ctx, EntityID entity);

/// @brief user system prototype
typedef struct {
  SystemUpdateFunc Update;
  ComponentMask Sig;
  SystemCategory Category;
  bool Active;
} System;

/// @brief single component prototype
typedef struct {
  void* Data; // arbitrary data for all entities
  size_t ElementSize;
} ComponentPool;

typedef struct {
  // Entities
  ComponentMask entity_masks[ECS_MAX_ENTITIES];
  bool entity_active[ECS_MAX_ENTITIES];

  // Components
  ComponentPool component_pools[ECS_MAX_COMPONENTS];

  // Systems
  System systems[ECS_MAX_SYSTEMS];
  uint32_t systems_count;
} ECSContext;

void ecs_init(ECSContext* ctx);
EntityID ecs_create_entity(ECSContext* ctx);

/// @brief register component
/// @param ctx ecs instance
/// @param comp_id index of component in global pool
/// @param size size of single component element
/// @return component mask
ComponentMask ecs_register_component(ECSContext* ctx, uint8_t comp_id, size_t size);

void* ecs_get_component(ECSContext* ctx, EntityID entity, uint8_t comp_id);

void ecs_register_system(ECSContext*, System*);
void ecs_update_systems(ECSContext* ctx, SystemCategory cat);