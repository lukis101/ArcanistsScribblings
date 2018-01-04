#ifndef OBJECT_TYPES_H
#define OBJECT_TYPES_H

#include <memory>

/// List of all game object types
/// Basic
#include "classes/c_baseclass.h"
typedef std::shared_ptr<CBaseClass> BaseClass;
#include "classes/c_material.h"
typedef std::shared_ptr<CMaterial> Material;

#include "classes/c_timer.h"
typedef CTimer* Timer;
#include "classes/c_camera.h"
typedef CCamera* Camera;

/// Entities
#include "classes/entity/c_player.h"
typedef std::shared_ptr<CPlayer> Player;

/// GUI
#include "classes/gui/c_button.h"
typedef std::shared_ptr<CButton> Button;
//#include "classes/gui/c_text.h"
//typedef std::shared_ptr<CText> Text;
//#include "classes/gui/c_slider.h"
//typedef std::shared_ptr<CSlider> Slider;

#endif
