#include "model.h"

#include <stdlib.h>

void MdlPtr_Free(struct Model* m) {
  if (!m) {
    return;
  }
  if (m->Vertices) free(m->Vertices);
  if (m->Indices) free(m->Indices);
  free(m);
}

void Mdl_Free(struct Model m) {
  if (m.Vertices) free(m.Vertices);
  if (m.Indices) free(m.Indices);
}
