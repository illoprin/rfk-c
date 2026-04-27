#pragma once

#include <stdint.h>
#include <kernel/core/types.h>
#include <vector/vector.h>

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
struct System {
  SystemUpdateFunc update_func;
  ComponentMask signature;
  SystemCategory category;
  bool active;
};

/// @brief single component prototype
struct ComponentPool {
  void* data; // arbitrary data for all entities
  size_t element_size;
};

struct ECSContext {
  // Entities
  ComponentMask entity_masks[ECS_MAX_ENTITIES];
  bool entity_active[ECS_MAX_ENTITIES];

  // Components
  struct ComponentPool component_pools[ECS_MAX_COMPONENTS];

  // Systems
  struct System systems[ECS_MAX_SYSTEMS];
  uint32_t systems_count;
};

// API
void ECS_Init(struct ECSContext* ctx);
EntityID ECS_EntityCreate(struct ECSContext* ctx);

/// @brief register component
/// @param ctx esc instance
/// @param comp_id index of component in global pool
/// @param size size of single component element
/// @return component mask
ComponentMask ECS_RegisterComponent(struct ECSContext* ctx, uint8_t comp_id, size_t size);

void* ECS_GetComponent(struct ECSContext* ctx, EntityID entity, uint8_t comp_id);

void ECS_RegisterSystem(struct ECSContext*, struct System*);
void ECS_UpdateSystems(struct ECSContext* ctx, SystemCategory cat);