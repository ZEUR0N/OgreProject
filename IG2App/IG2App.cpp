#include "IG2App.h"

#include <OgreEntity.h>
#include <OgreInput.h>
#include <OgreMovablePlane.h>
#include <SDL_keycode.h>
#include <OgreMeshManager.h>

using namespace Ogre;

bool IG2App::keyPressed(const OgreBites::KeyboardEvent& evt)
{
  if (evt.keysym.sym == SDLK_ESCAPE)
  {
    getRoot()->queueEndRendering();
  }
  else if (evt.keysym.sym == SDLK_g) {
	  if(ClockNode != nullptr) ClockNode->roll(Degree(-5));
	  
  }
  else if (evt.keysym.sym == SDLK_h) {
	  nodo_ficticio();
  }
  else if (evt.keysym.sym == SDLK_j) {
	  giro_truco();
  }
  
  return true;
}

void IG2App::nodo_ficticio()
{
	if (ficticioDronNode != nullptr) {
		ficticioDronNode->translate(0, -420, 0, SceneNode::TS_LOCAL);
		ficticioDronNode->roll(Degree(-5), SceneNode::TS_LOCAL);
		ficticioDronNode->translate(0, 420, 0, SceneNode::TS_LOCAL);
	}
}

void IG2App::giro_truco()
{
	if (ficticioDronNode != nullptr)
		ficticioDronNode->yaw(Degree(-5), SceneNode::TS_LOCAL);
}

void IG2App::Scene1()
{
#pragma region Scena1
	#pragma region Clock
	////Reloj
	ClockNode = mSM->getRootSceneNode()->createChildSceneNode("Clock");
	SpheresNode = ClockNode->createChildSceneNode("Horas");
	float ang = 90;
	for (int i = 0; i < mHourNode.size(); i++)
	{
		mHourNode[i] = SpheresNode->createChildSceneNode("Hora" + std::to_string(i));
		Ogre::Entity* hour = mSM->createEntity("sphere.mesh");
		mHourNode[i]->setPosition(Ogre::Math::Cos(Ogre::Math::DegreesToRadians(ang))*250, Ogre::Math::Sin(Ogre::Math::DegreesToRadians(ang))*250, 0);
		//if (i %2 == 0) 
			mHourNode[i]->setScale(0.35, 0.35, 0.35);
		//else
		//	mHourNode[i]->setScale(0.25, 0.25, 0.25);
		mHourNode[i]->attachObject(hour);
		ang += 360.0 / mHourNode.size();
	}
	////Aguja
	std::array<SceneNode*,3> agujaNode;

	for (int i = 0; i < agujaNode.size(); i++) {
		agujaNode[i] = ClockNode->createChildSceneNode("Aguja_" + std::to_string(i));
		Ogre::Entity* aguja = mSM->createEntity("column.mesh");
		float n;
		switch (i)
		{
		case 0:
			n = 1;
			break;
		case 1:
			n = 0;
			break;
		case 2:
			n = 2;
			break;
		}
		agujaNode[i]->roll(Ogre::Degree(-90*n));
		agujaNode[i]->attachObject(aguja);
	}
	mSM->getSceneNode("Aguja_0")->setScale(0.5, 0.3, 0.5);	//Horas
	mSM->getSceneNode("Aguja_0")->translate(20, 0, 0);
	mSM->getSceneNode("Aguja_1")->setScale(0.4, 0.4, 0.4);	//Minutos
	mSM->getSceneNode("Aguja_1")->translate(0, 20, 0);
	mSM->getSceneNode("Aguja_2")->roll(Degree(-30));
	mSM->getSceneNode("Aguja_2")->setScale(0.2, 0.5, 0.2);	//Segundos
	mSM->getSceneNode("Aguja_2")->translate(-10, -20, 0);

	#pragma endregion
	#pragma region Molino
	SceneNode* molinoNode = mSM->getRootSceneNode()->createChildSceneNode("Molino");
	molino = new Molino(molinoNode);
	molinoNode->translate(-1200, 0, 0);
	addInputListener(molino);
	#pragma endregion
	#pragma region Dron
	SceneNode* dronNode = mSM->getRootSceneNode()->createChildSceneNode("Dron");
	dron = new Dron(dronNode);
	dronNode->translate(800, 0, 0);
	addInputListener(dron);
	#pragma endregion
#pragma endregion
}

void IG2App::Scene2()
{
#pragma region Scena2
	#pragma region Planeta
	planetaNode = mSM->getRootSceneNode()->createChildSceneNode("Planeta");
	Ogre::Entity* esfera = mSM->createEntity("sphere.mesh");
	planetaNode->setScale(4, 4, 4);
	planetaNode->attachObject(esfera);
	ficticioDronNode = mSM->getRootSceneNode()->createChildSceneNode("Dron2");
	ficticioDronNode->translate(0, 420, 0);
	ficticioDronNode->scale(0.1, 0.1, 0.1);
	dron = new Dron(ficticioDronNode);
	addInputListener(dron);

	#pragma endregion
	#pragma region Avion
	SceneNode* avionNode = mSM->getRootSceneNode()->createChildSceneNode("Avion");
	avion = new Avion(avionNode);
	avionNode->translate(0, 425, 0);
	avionNode->setScale(0.2, 0.2, 0.2);
	addInputListener(avion);
	#pragma endregion
	#pragma region Malla
	SceneNode* planoNode = mSM->getRootSceneNode()->createChildSceneNode("Plano");
	plano = new Plano(planoNode);
	planoNode->pitch(Degree(90));
	planoNode->translate(0, 0, -800);
	addInputListener(plano);
	#pragma endregion
#pragma endregion
}

void IG2App::Scene3()
{
#pragma region Scena3
#pragma region Planeta
	planetaNode = mSM->getRootSceneNode()->createChildSceneNode("Planeta");
	Ogre::Entity* esfera = mSM->createEntity("sphere.mesh");
	planetaNode->setScale(4, 4, 4);
	planetaNode->attachObject(esfera);
#pragma endregion

#pragma region Enjambre
	controlDronNode = mSM->getRootSceneNode()->createChildSceneNode("DronControl");
	controlDronNode->translate(0, 420, 0);
	controlDronNode->scale(0.08, 0.08, 0.08);
	dron = new Dron(controlDronNode);
	addInputListener(dron);

	for (int i = 0; i < 24; ++i) {
		SceneNode* n = mSM->getRootSceneNode()->createChildSceneNode();
		n->translate(0, 420, 0);
		n->scale(0.04, 0.04, 0.04);
		Dron* d = new Dron(n, false);
		addInputListener(d);
		mini_drones.push_back(d);
		enjambre.push_back(n);
	}

#pragma endregion

// Avion
	SceneNode* avionNode = mSM->getRootSceneNode()->createChildSceneNode("Avion");
	avion = new Avion(avionNode);
	avionNode->translate(0, 425, 0);
	avionNode->setScale(0.2, 0.2, 0.2);
	addInputListener(avion);

#pragma region Malla
	SceneNode* planoNode = mSM->getRootSceneNode()->createChildSceneNode("Plano");
	plano = new Plano(planoNode);
	planoNode->pitch(Degree(90));
	planoNode->translate(0, 0, -800);
	addInputListener(plano);
#pragma endregion
#pragma endregion
}

void IG2App::Scene4()
{
#pragma region Scena4
//Skyplane
	// 	   2
	//mSM->setSkyPlane(true, Plane(Vector3::UNIT_Z, -100), "IG2App/space", 2, 1, true, 0.5, 100, 100);
	// 	   9
	//mSM->setSkyPlane(true, Plane(Vector3::UNIT_Z, -100), "Practica2/SpaceSky", 2, 1, true, 0.5, 100, 100);
	// 	   10
	mSM->setSkyPlane(true, Plane(Vector3::UNIT_Z, -100), "Practica2/Space", 2, 1, true, 0.5, 100, 100);
	
	
// Rio
	SceneNode* rioNode = mSM->getRootSceneNode()->createChildSceneNode("Rio");
	rio = new Rio(rioNode);	//1200 x 800
	rioNode->translate(0, -10, 0);
	addInputListener(rio);

//Espejo
	SceneNode* espejoNode = mSM->getRootSceneNode()->createChildSceneNode("Espejo");
	esp = new Espejo(espejoNode, "1");
	espejoNode->roll(Degree(90));
	espejoNode->translate(800, 0, 0);
	addInputListener(esp);
	//Techo
	SceneNode* espejoTechoNode = mSM->getRootSceneNode()->createChildSceneNode("EspejoTecho");
	espTecho = new Espejo(espejoTechoNode, "2");
	espejoTechoNode->translate(0, 800, 0);
	espejoTechoNode->pitch(Degree(180));
	addInputListener(espTecho);

// Plataforma Amarilla
	SceneNode* plat1Node = mSM->getRootSceneNode()->createChildSceneNode("Plat1");
	plataforma1 = new Plataforma(plat1Node, 2, 2, Plataforma::YELLOW);
	plat1Node->translate(-480, -10, 280);
	addInputListener(plataforma1);

// Plataforma Roja
	/*SceneNode* plat2Node = mSM->getRootSceneNode()->createChildSceneNode("Plat2");
	plataforma2 = new Plataforma(plat2Node, 2, 2, Plataforma::RED);
	plat2Node->translate(480,-10, -280);
	addInputListener(plataforma2);*/

// Sinbad
	SceneNode* sinbadNode = mSM->getRootSceneNode()->createChildSceneNode("Sinbad");
	sinbad = new Sinbad(sinbadNode);
	addInputListener(sinbad);

// Avion
	SceneNode* avionNode = mSM->getRootSceneNode()->createChildSceneNode("Avion");
	avion = new Avion(avionNode);
	avionNode->translate(-280, 400, 0);
	avionNode->setScale(0.2, 0.2, 0.2);
	addInputListener(avion);

// Bomba
	SceneNode* bombaNode = mSM->getRootSceneNode()->createChildSceneNode("Bomba");
	bomba = new Bomba(bombaNode);
	bombaNode->translate(0, -10, 0);
	addInputListener(bomba);

// Esfera
	esferaNode = mSM->getRootSceneNode()->createChildSceneNode();
	Entity* esfera = mSM->createEntity("sphere.mesh");
	esfera->setMaterialName("IG2App/billboard");
	esferaNode->setScale(0.2, 0.2, 0.2);
	esferaNode->translate(480, 20, -280);
	esferaNode->attachObject(esfera);

	
//Reflejo
	Camera* camRef = mSM->createCamera("RefCam");
	camRef->setNearClipDistance(1);
	camRef->setFarClipDistance(10000);
	camRef->setAutoAspectRatio(true);
	mCamNode->attachObject(camRef);

	rio->setReflejo(camRef, cam->getViewport()->getActualWidth(), cam->getViewport()->getActualHeight());

	Camera* camRef3 = mSM->createCamera("RefCam3");
	camRef3->setNearClipDistance(1);
	camRef3->setFarClipDistance(10000);
	camRef3->setAutoAspectRatio(true);
	mCamNode->attachObject(camRef3);
	espTecho->setEspejoTecho(camRef3, cam->getViewport()->getActualWidth(), cam->getViewport()->getActualHeight());

	Camera* camRef2 = mSM->createCamera("RefCam2");
	camRef2->setNearClipDistance(1);
	camRef2->setFarClipDistance(10000);
	camRef2->setAutoAspectRatio(true);
	mCamNode->attachObject(camRef2);
	esp->setEspejo(camRef2, cam->getViewport()->getActualWidth(), cam->getViewport()->getActualHeight());


#pragma endregion
}

void IG2App::shutdown()
{
  mShaderGenerator->removeSceneManager(mSM);  
  mSM->removeRenderQueueListener(mOverlaySystem);  
					
  mRoot->destroySceneManager(mSM);  

  delete mTrayMgr;  mTrayMgr = nullptr;
  delete mCamMgr; mCamMgr = nullptr;
  delete molino; molino = nullptr;
  delete dron; dron = nullptr;
  delete avion; avion = nullptr;
  delete plano; plano = nullptr;
  delete rio; rio = nullptr;
  delete plataforma1; plataforma1 = nullptr;
  delete plataforma2; plataforma2 = nullptr;
  delete sinbad; sinbad = nullptr;
  delete bomba; bomba = nullptr;
  delete cam; cam = nullptr;
  delete esp; esp = nullptr;
  delete espTecho; espTecho = nullptr;
  
  for (auto d : mini_drones)
	  delete d;
  // do not forget to call the base 
  IG2ApplicationContext::shutdown();
}

void IG2App::setup(void)
{
  // do not forget to call the base first
  IG2ApplicationContext::setup();

  mSM = mRoot->createSceneManager();  

  // register our scene with the RTSS
  mShaderGenerator->addSceneManager(mSM);

  mSM->addRenderQueueListener(mOverlaySystem);

  mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);  
  mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
  addInputListener(mTrayMgr);

  addInputListener(this);   
  setupScene();
}

void IG2App::setupScene(void)
{
	// create the camera
	cam = mSM->createCamera("Cam");
	cam->setNearClipDistance(1);
	cam->setFarClipDistance(10000);
	cam->setAutoAspectRatio(true);
	//cam->setPolygonMode(Ogre::PM_WIREFRAME); 

	mCamNode = mSM->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(cam);

	mCamNode->setPosition(0, -400, 1000);
	mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
	//mCamNode->setDirection(Ogre::Vector3(0, 0, -1));  

	// and tell it to render into the main window
	Viewport* vp = getRenderWindow()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(0.6,0.7,0.8));

	//------------------------------------------------------------------------

	// without light we would just get a black screen 

	Light* luz = mSM->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(1.0, 1.0, 1.0);

	mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
	//mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);

	mLightNode->setDirection(Ogre::Vector3(0, -1, -1));  //vec3.normalise();
	//lightNode->setPosition(0, 0, 1000);

	

	//------------------------------------------------------------------------
	///Render Scene
	Scene4();
	//------------------------------------------------------------------------

	mCamMgr = new OgreBites::CameraMan(mCamNode);
	addInputListener(mCamMgr);
	mCamMgr->setStyle(OgreBites::CS_ORBIT);

	//mCamMgr->setTarget(mSinbadNode);  
	//mCamMgr->setYawPitchDist(Radian(0), Degree(30), 100);

	//------------------------------------------------------------------------

}


