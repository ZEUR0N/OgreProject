#ifndef __IG2App_H__
#define __IG2App_H__

#include "IG2ApplicationContext.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "./GameObjects/GameObjects.h"
#include <OgreTrays.h>
#include <OgreCameraMan.h>


class IG2App : public  OgreBites::IG2ApplicationContext, OgreBites::InputListener 
{
public:
  explicit IG2App() : IG2ApplicationContext("IG2_Practica1") { };  // new -> setup()  
  virtual ~IG2App() { };   // delete -> shutdown()  
 
protected:
  virtual void setup();
  virtual void shutdown();
  virtual void setupScene();

  virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);  // InputListener

  //Metodos pochos
  void nodo_ficticio();
  void giro_truco();
  void Scene1();
  void Scene2();
  void Scene3();
  void Scene4();

      
  Ogre::SceneManager* mSM = nullptr;
  OgreBites::TrayManager* mTrayMgr = nullptr;    
  Ogre::SceneNode* mLightNode = nullptr;
  Ogre::SceneNode* mCamNode = nullptr;
  Ogre::SceneNode* mSinbadNode = nullptr; 
  Ogre::SceneNode* esferaNode = nullptr;
  OgreBites::CameraMan* mCamMgr = nullptr;
  //Entity
  Ogre::SceneNode* ClockNode = nullptr;
  Ogre::SceneNode* SpheresNode = nullptr;
  std::array<Ogre::SceneNode*, 12> mHourNode;
  SceneNode* planetaNode = nullptr, *ficticioDronNode = nullptr;
  //Enjambre
  SceneNode* controlDronNode = nullptr;
  std::vector<Ogre::SceneNode*> enjambre;
  std::vector<Dron*> mini_drones;

  Molino* molino = nullptr;
  Dron* dron = nullptr;
  Avion* avion = nullptr;
  Plano* plano = nullptr;
  Rio* rio = nullptr;
  Espejo* esp = nullptr;
  Espejo* espTecho = nullptr;
  Sinbad* sinbad = nullptr;
  Bomba* bomba = nullptr;
  Plataforma* plataforma1 = nullptr;
  Plataforma* plataforma2 = nullptr;
  Camera* cam = nullptr;
};

#endif
