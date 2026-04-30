#include "obj.h"

#include <vector/vector.h>
#include <kernel/core/log.h>
#include <kernel/core/defs.h>
#include <stdio.h>
#include <string.h>

struct ObjRawData {
  Vector Vertices;
  Vector Texcoord;
  Vector Normals;
};

struct FaceIndices {
  int VertexIndex;
  int TexcoordIndex;
  int NormalIndex;
};

// parse 'v' string
void ObjParseVertices(int lineNum, char* line, struct ObjRawData* obj) {
  if (line == NULL) return;

  float x, y, z;
  if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
    Vec_Push(&obj->Vertices, x);
    Vec_Push(&obj->Vertices, y);
    Vec_Push(&obj->Vertices, z);
  }
  else {
    LogWarn("failed to parse line %d");
  }
}

// parse 'vt' string
void ObjParseTexcoords(int lineNum, char* line, struct ObjRawData* obj) {
  if (line == NULL) return;

  float x, y;
  if (sscanf(line, "vt %f %f", &x, &y) == 2) {
    Vec_Push(&obj->Texcoord, x);
    Vec_Push(&obj->Texcoord, y);
  }
  else {
    LogWarn("failed to parse line %d");
  }
}

// parse 'vn' string
void ObjParseNormals(int lineNum, char* line, struct ObjRawData* obj) {
  if (line == NULL) return;

  float x, y, z;
  if (sscanf(line, "vn %f %f %f", &x, &y, &z) == 3) {
    Vec_Push(&obj->Normals, x);
    Vec_Push(&obj->Normals, y);
    Vec_Push(&obj->Normals, z);
  }
  else {
    LogWarn("failed to parse line %d");
  }
}

// parse 'f' string
// adds vertices and indices to final array
int ObjParseFace(
  int line,
  char* lineStr,
  struct ObjRawData* obj,
  Vector* modelVertices,
  Vector* modelIndices
) {
  // skip 'f ' token
  char* token = strtok(lineStr + 2, " ");
  if (!token) return 1;

  // parse face entry
  struct FaceIndices face[4];
  int idx = 0;
  while (token != NULL) {
    if (idx >= 4) return 1;

    int vi, vti, vni;
    if (sscanf(token, "%u/%u/%u", &vi, &vti, &vni) != 3) {
      LogErr("failed parse face indices on line %d", line);
      return 1;
    }

    face[idx] = (struct FaceIndices){
      .VertexIndex = vi - 1,
      .TexcoordIndex = vti - 1,
      .NormalIndex = vni - 1
    };
    idx++;
    token = strtok(NULL, " "); // goto next entry
  }

  if (idx < 3) {
    LogErr("invalid face on line %d", line);
    return 1;
  };

  // add model vertices and indices
  for (int i = 0; i < idx; i++) {
    struct FaceIndices f = face[i];
    // get vertex
    float vx = Vec_At(&obj->Vertices, float, f.VertexIndex * 3);
    float vy = Vec_At(&obj->Vertices, float, f.VertexIndex * 3 + 1);
    float vz = Vec_At(&obj->Vertices, float, f.VertexIndex * 3 + 2);

    // get texcoord
    float tu = Vec_At(&obj->Texcoord, float, f.TexcoordIndex * 2);
    float tv = Vec_At(&obj->Texcoord, float, f.TexcoordIndex * 2 + 1);

    // get normal
    float nx = Vec_At(&obj->Normals, float, f.NormalIndex * 3);
    float ny = Vec_At(&obj->Normals, float, f.NormalIndex * 3 + 1);
    float nz = Vec_At(&obj->Normals, float, f.NormalIndex * 3 + 2);

    // add model vertex
    int index = modelVertices->Len; // current index is len of vertices array
    struct ModelVertex v = {
      .Position = { vx, vy, vz },
      .Texcoords = { tu, tv },
      .Normal = { nx, ny, nz }
    };
    Vec_Push(modelVertices, v);
    Vec_Push(modelIndices, index);
  }
  return 0;
}

void ObjAllocate(struct ObjRawData* raw) {
  Vec_Init(&raw->Vertices, float);
  Vec_Init(&raw->Texcoord, float);
  Vec_Init(&raw->Normals, float);
}

void ObjFree(struct ObjRawData* raw) {
  Vec_Destroy(&raw->Vertices);
  Vec_Destroy(&raw->Texcoord);
  Vec_Destroy(&raw->Normals);
}

int Mdl_InitFromObj(struct Model* mdl, const char* path) {
  FILE* f = fopen(path, "r");
  if (!f) {
    LogErr("Mdl_FromObj - could not open file \"%s\"", path);
    return 1;
  }

  // create temp obj storage
  struct ObjRawData raw;
  ObjAllocate(&raw);

  // final vertices arrays
  Vector vertices;
  Vec_Init(&vertices, struct ModelVertex);

  // final indices array
  Vector indices;
  Vec_Init(&indices, int);

  char lineStr[MAX_LINE_LEN];
  int line = 0;

  while (fgets(lineStr, sizeof(lineStr), f)) {
    // skip spaces and comments
    if (lineStr[0] == '\n' || lineStr[0] == '#' || lineStr[0] == '\0')
      continue;

    if (!strncmp(lineStr, "v ", 2)) {
      ObjParseVertices(line, lineStr, &raw);
    }
    else if (!strncmp(lineStr, "vt ", 3)) {
      ObjParseTexcoords(line, lineStr, &raw);
    }
    else if (!strncmp(lineStr, "vn ", 3)) {
      ObjParseNormals(line, lineStr, &raw);
    }
    else if (!strncmp(lineStr, "f ", 2)) {
      if (ObjParseFace(line, lineStr, &raw, &vertices, &indices)) {
        ObjFree(&raw);
        return 1;
      }
    }

    line++;
  }

  fclose(f);

  ObjFree(&raw);

  mdl->Vertices = vertices.Data;
  mdl->NumVertices = vertices.Len;
  mdl->Indices = indices.Data;
  mdl->NumIndices = indices.Len;

  return 0;
}