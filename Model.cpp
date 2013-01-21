#include "Model.h"

Model::Model() {
  this->m_mode = eStatic;
}

void Model::nextMode() {
  switch (this->m_mode) {
    case eStatic: m_mode = eSlow; break;
    case eSlow: m_mode = eFast; break;
    case eFast: m_mode = eStatic; break;
    case eSetup: m_mode = eStatic; break;
  }
}

Model::EMode Model::getMode() {
  return m_mode;
}

void Model::setup() {
  this->m_mode = eSetup;
}

