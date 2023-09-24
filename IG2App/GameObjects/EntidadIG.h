#pragma once


#include <OgreInput.h>
#include <OgreMovablePlane.h>
#include <OgreRenderTargetListener.h>
#include <OgreRenderTexture.h>
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreSubEntity.h>
#include <OgreTechnique.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreCameraMan.h>
#include <OgreViewport.h>

using namespace Ogre;
struct MessageType {
	std::string type;
};
class EntidadIG :public OgreBites::InputListener {
public:
	//Constructora y destructora
	EntidadIG(SceneNode* node);
	~EntidadIG();
	//Vector estático de listeners
	static std::vector<EntidadIG*> appListeners;
	//Añadir entidad como listener al vector con push_back()
	static void addListener(EntidadIG* entidad);
	
	Ogre::SceneNode* mNode;
protected:
	Ogre::SceneManager* mSM;
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) { return false; };
	virtual void frameRendered(const Ogre::FrameEvent& evt) {};
	void sendEvent(MessageType msgType, EntidadIG* entidad = nullptr);
	virtual void receiveEvent(MessageType msgType, EntidadIG* entidad) {};
};