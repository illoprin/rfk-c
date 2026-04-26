#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel/core/types.h>
#include <glad/glad.h>

// VAO = rule for reading buffer
struct rhi_VAO {
  uint ID;

  // Cached for validation/debug
  uint ebo_id;
};

// Lifecycle

void rhi_VAO_Create(struct rhi_VAO* vao);
void rhi_VAO_Destroy(struct rhi_VAO* vao);

// Binding

void rhi_VAO_Bind(struct rhi_VAO* vao);

// REQUIRED INDEX BUFFER (EBO)

void rhi_VAO_SetIndexBuffer(
  struct rhi_VAO* vao,
  uint32_t ebo_id
);

// Data source
void rhi_VAO_BindVertexBuffer(
  struct rhi_VAO* vao,
  uint32_t binding_index,
  uint32_t vbo_id,
  size_t offset,
  size_t stride
);

// Attribute description

typedef enum {
  RHI_ATTR_FLOAT,
  RHI_ATTR_INT,
  RHI_ATTR_UNSIGNED_INT
} rhi_AttributeType;

struct rhi_Attribute {
  size_t            Offset;
  uint              Location;
  uint              BindingIndex;
  uint              Components;
  rhi_AttributeType Type;
  bool              Normalized;
  uint              Divisor;
};

void rhi_VAO_SetAttribute(
  struct rhi_VAO* vao,
  struct rhi_Attribute attr
);

// Validation / safety

bool rhi_VAO_IsValid(const struct rhi_VAO vao);