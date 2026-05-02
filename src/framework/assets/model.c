#include "model.h"

#include <stdlib.h>

void mdl_ptr_free(Model* m) {
  if (!m) {
    return;
  }
  if (m->Vertices) free(m->Vertices);
  if (m->Indices) free(m->Indices);
  free(m);
}

void mdl_free(Model m) {
  if (m.Vertices) free(m.Vertices);
  if (m.Indices) free(m.Indices);
}