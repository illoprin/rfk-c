#include "obj.h"

#include <rfklib/log.h>
#include <rfklib/vector.h>

#include <kernel/core/defs.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  Vector Vertices;
  Vector Texcoord;
  Vector Normals;
} ObjRawData;

typedef struct {
  int VertexIndex;
  int TexcoordIndex;
  int NormalIndex;
} FaceIndices;

// parse 'v' string
void obj_parse_vertices(int lineNum, char* line, ObjRawData* obj) {
  if (line == NULL) { return; }

  float x, y, z;
  if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
    vec_push(&obj->Vertices, x);
    vec_push(&obj->Vertices, y);
    vec_push(&obj->Vertices, z);
  } else {
    LogWarn("failed to parse line %d");
  }
}

// parse 'vt' string
void obj_parse_texcoords(int lineNum, char* line, ObjRawData* obj) {
  if (line == NULL) { return; }

  float x, y;
  if (sscanf(line, "vt %f %f", &x, &y) == 2) {
    vec_push(&obj->Texcoord, x);
    vec_push(&obj->Texcoord, y);
  } else {
    LogWarn("failed to parse line %d");
  }
}

// parse 'vn' string
void obj_parse_normals(int lineNum, char* line, ObjRawData* obj) {
  if (line == NULL) { return; }

  float x, y, z;
  if (sscanf(line, "vn %f %f %f", &x, &y, &z) == 3) {
    vec_push(&obj->Normals, x);
    vec_push(&obj->Normals, y);
    vec_push(&obj->Normals, z);
  } else {
    LogWarn("failed to parse line %d");
  }
}

// parse 'f' string
// adds vertices and indices to final array
int obj_parse_face(
  int line, char* lineStr, ObjRawData* obj, Vector* modelVertices,
  Vector* modelIndices
) {
  // skip 'f ' token
  char* token = strtok(lineStr + 2, " ");
  if (!token) { return 1; }

  // parse face entry
  FaceIndices face[4];
  int         idx = 0;
  while (token != NULL) {
    if (idx >= 4) { return 1; }

    int vi, vti, vni;
    if (sscanf(token, "%u/%u/%u", &vi, &vti, &vni) != 3) {
      LogErr("failed parse face indices on line %d", line);
      return 1;
    }

    face[idx] = (FaceIndices){.VertexIndex   = vi - 1,
                              .TexcoordIndex = vti - 1,
                              .NormalIndex   = vni - 1};
    idx++;
    token = strtok(NULL, " ");  // goto next entry
  }

  if (idx < 3) {
    LogErr("invalid face on line %d", line);
    return 1;
  };

  // add model vertices and indices
  for (int i = 0; i < idx; i++) {
    FaceIndices f = face[i];
    // get vertex
    float vx = vec_get(&obj->Vertices, float, f.VertexIndex * 3);
    float vy = vec_get(&obj->Vertices, float, f.VertexIndex * 3 + 1);
    float vz = vec_get(&obj->Vertices, float, f.VertexIndex * 3 + 2);

    // get texcoord
    float tu = vec_get(&obj->Texcoord, float, f.TexcoordIndex * 2);
    float tv =
      vec_get(&obj->Texcoord, float, f.TexcoordIndex * 2 + 1);

    // get normal
    float nx = vec_get(&obj->Normals, float, f.NormalIndex * 3);
    float ny = vec_get(&obj->Normals, float, f.NormalIndex * 3 + 1);
    float nz = vec_get(&obj->Normals, float, f.NormalIndex * 3 + 2);

    // add model vertex
    int index =
      modelVertices->Len;  // current index is len of vertices array
    ModelVertex v = {
      .Position  = {vx, vy, vz},
      .Texcoords = {tu, tv},
      .Normal    = {nx, ny, nz}
    };
    vec_push(modelVertices, v);
    vec_push(modelIndices, index);
  }
  return 0;
}

void obj_init(ObjRawData* raw) {
  vec_init(&raw->Vertices, float);
  vec_init(&raw->Texcoord, float);
  vec_init(&raw->Normals, float);
}

void obj_destroy(ObjRawData* raw) {
  vec_destroy(&raw->Vertices);
  vec_destroy(&raw->Texcoord);
  vec_destroy(&raw->Normals);
}

int mdl_init_from_obj(Model* mdl, const char* path) {
  FILE* f = fopen(path, "r");
  if (!f) {
    LogErr("Mdl_FromObj - could not open file \"%s\"", path);
    return 1;
  }

  // create temp obj storage
  ObjRawData raw;
  obj_init(&raw);

  // final vertices arrays
  Vector vertices;
  vec_init(&vertices, ModelVertex);

  // final indices array
  Vector indices;
  vec_init(&indices, int);

  char lineStr[MAX_LINE_LEN];
  int  line = 0;

  while (fgets(lineStr, sizeof(lineStr), f)) {
    // skip spaces and comments
    if (lineStr[0] == '\n' || lineStr[0] == '#'
        || lineStr[0] == '\0') {
      continue;
    }

    if (!strncmp(lineStr, "v ", 2)) {
      obj_parse_vertices(line, lineStr, &raw);
    } else if (!strncmp(lineStr, "vt ", 3)) {
      obj_parse_texcoords(line, lineStr, &raw);
    } else if (!strncmp(lineStr, "vn ", 3)) {
      obj_parse_normals(line, lineStr, &raw);
    } else if (!strncmp(lineStr, "f ", 2)) {
      if (obj_parse_face(line, lineStr, &raw, &vertices, &indices)) {
        obj_destroy(&raw);
        return 1;
      }
    }

    line++;
  }

  fclose(f);

  obj_destroy(&raw);

  mdl->Vertices      = vertices.Data;
  mdl->CountVertices = vertices.Len;
  mdl->Indices       = indices.Data;
  mdl->CountIndices  = indices.Len;

  return 0;
}