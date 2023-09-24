#include "GameObjects.h"
#include <SDL_keycode.h>
#include <OgreMeshManager.h>
#include <OgreFrameListener.h>
#include <OgreTimer.h>
#include <OgreAnimation.h>
#include <OgreKeyFrame.h>
#include <OgreBillboardSet.h>
#include <OgreParticleSystem.h>

Aspa::Aspa(SceneNode* node, bool _con_Adorno) : EntidadIG(node)
{
	//tabla
	SceneNode* tableroNode = mNode->createChildSceneNode();
	Entity* rect = mSM->createEntity("cube.mesh");
	rect->setMaterialName("IG2App/shBrown");
	tableroNode->setScale(8, 1.3, 0.1);
	tableroNode->translate(200, 0, 0);
	tableroNode->attachObject(rect);

	//Fin Aspa
	if (_con_Adorno) {
		SceneNode* adornoNode = mNode->createChildSceneNode();
		Entity* cili = mSM->createEntity("Barrel.mesh");
		cili->setMaterialName("IG2App/shRed");
		adornoNode->setScale(6, 16, 6);
		adornoNode->translate(560, 0, 14);
		adornoNode->setInheritOrientation(false);
		adornoNode->attachObject(cili);
	}
}
//-----------------------------------------------------------------------------------------

AspasMolino::AspasMolino(SceneNode* node, bool _conAd, bool sent_hor, int asp) : EntidadIG(node)
{
	numAspas = asp;
	sent_horario = sent_hor;

	//Cilindro
	centroNode = mNode->createChildSceneNode();
	Entity* centro = mSM->createEntity("Barrel.mesh");
	centro->setMaterialName("IG2App/woodPallet");
	centroNode->setScale(40, 20, 40);
	centroNode->pitch(Degree(90));
	centroNode->translate(0, 0, -40);
	centroNode->attachObject(centro);
	//Aspas
	float ang = 0;
	for (int i = 0; i < numAspas; i++)
	{
		SceneNode* nAspa = mNode->createChildSceneNode();
		Aspa* a = new Aspa(nAspa, _conAd);
		aspas.push_back(a);

		nAspa->roll(Degree(ang));
		ang += 360.0 / numAspas;
	}
}

bool AspasMolino::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_g) {
		rotarAspas(8);
	}
	if (evt.keysym.sym == SDLK_c) {
		centroNode->translate(0, 0, -5);
	}
	return true;
}

void AspasMolino::rotarAspas(int grad)
{
	float degree = (sent_horario) ? grad : -grad;
	mNode->roll(Degree(degree));
}


//-----------------------------------------------------------------------------------------

Molino::Molino(SceneNode* node) : EntidadIG(node)
{

	//Torre
	torreNode = mNode->createChildSceneNode();
	Entity* torre = mSM->createEntity("Barrel.mesh");
	torreNode->setScale(120, 120, 120);
	torreNode->translate(0, -400, 0);
	torreNode->attachObject(torre);

	//EsferaTecho
	esferaNode = mNode->createChildSceneNode();
	Entity* esfera = mSM->createEntity("sphere.mesh");
	esferaNode->setScale(3, 3, 3);
	esferaNode->attachObject(esfera);

	//Aspas
	aspasNode = mNode->createChildSceneNode();
	aspasMolino = new AspasMolino(aspasNode);
	aspasNode->translate(0, 0, 340);

}

bool Molino::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	aspasMolino->keyPressed(evt);

	if (evt.keysym.sym == SDLK_h) {
		aspasNode->translate(0, 0, -340, SceneNode::TS_LOCAL);
		aspasNode->yaw(Degree(5), SceneNode::TS_PARENT);
		aspasNode->translate(0, 0, 340, SceneNode::TS_LOCAL);

	}

	return true;
}
//-----------------------------------------------------------------------------------------

RotorDron::RotorDron(SceneNode* node, bool sent_hor, int asp) : EntidadIG(node)
{
	aspas = asp;
	//Esfera
	esferaNode = mNode->createChildSceneNode();
	Entity* esfera = mSM->createEntity("sphere.mesh");
	esfera->setMaterialName("IG2App/shOrange");
	esferaNode->setScale(3, 3, 3);
	esferaNode->attachObject(esfera);

	//Aspas
	helicesNode = mNode->createChildSceneNode();
	aspasMolino = new AspasMolino(helicesNode, false, sent_hor, aspas);
	helicesNode->translate(0, 300, 0);
	helicesNode->pitch(Degree(-90));
	helicesNode->scale(0.5, 0.5, 0.5);
}
bool RotorDron::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	aspasMolino->keyPressed(evt);
	return true;
}
void RotorDron::frameRendered(const Ogre::FrameEvent& evt)
{
	aspasMolino->rotarAspas(15);
}
//-----------------------------------------------------------------------------------------

BrazoDron::BrazoDron(SceneNode* node, bool sent_hor, int asp) : EntidadIG(node)
{
	aspas = asp;
	//Brazo
	cilindroNode = mNode->createChildSceneNode();
	Entity* cili = mSM->createEntity("Barrel.mesh");
	cili->setMaterialName("IG2App/tusk");
	cilindroNode->setScale(10, 50, 10);
	cilindroNode->roll(Degree(90));
	cilindroNode->translate(200, 0, 0);
	cilindroNode->attachObject(cili);

	//Rotor
	rotorNode = mNode->createChildSceneNode();
	rotorDron = new RotorDron(rotorNode, sent_hor, aspas);
	rotorNode->translate(350, 0, 0);
	rotorNode->scale(0.2, 0.2, 0.2);
}
bool BrazoDron::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	rotorDron->keyPressed(evt);
	return true;
}
void BrazoDron::frameRendered(const Ogre::FrameEvent& evt)
{
	rotorDron->frameRendered(evt);
}
//-----------------------------------------------------------------------------------------
int Dron::numdrones = 0;

Dron::Dron(SceneNode* node, bool main, int br, int asp) : EntidadIG(node)
{
	main_ = main,  brazos = br; aspas = asp;

	//Centro
	centroNode = mNode->createChildSceneNode();
	esfera = mSM->createEntity("sphere.mesh");
	if(main_)
		esfera->setMaterialName("IG2App/shRed");
	else 
		esfera->setMaterialName("IG2App/billboard");
	centroNode->setScale(0.8, 0.8, 0.8);
	centroNode->attachObject(esfera);

	//Brazos
	float ang = 0;
	for (int i = 0; i < brazos; i++)
	{
		SceneNode* nBrazo = mNode->createChildSceneNode();
		bool sent_hor = (i % 2 == 0) ? true : false;
		BrazoDron* b = new BrazoDron(nBrazo, sent_hor, aspas);
		brazosDron.push_back(b);

		nBrazo->yaw(Degree(ang));
		if (i == 0)
			nBrazo->scale(0.7, 0.85, 0.85);
		else
			nBrazo->scale(0.65, 0.65, 0.65);
		ang += 360.0 / brazos;
	}

	if (main_) {	//Si no es de un enjambre
		//Foco
		SceneNode* lightNode = mNode->createChildSceneNode();
		Light* luzFoco = mSM->createLight();
		luzFoco->setType(Ogre::Light::LT_SPOTLIGHT);
		luzFoco->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
		luzFoco->setDirection(Ogre::Vector3(0, -1, 0));
		luzFoco->setSpotlightRange(Degree(3.0f), Degree(15.0f), 10.0f);
		lightNode->attachObject(luzFoco);
	}

	//Timer
	myTimer = new Ogre::Timer();
	timer = myTimer->getMilliseconds() + 2000;
	stop = false;
	addListener(this);

	dir = Ogre::Math::RangeRandom(0, 1) < 0.5f ? -3 : 3;

	numdrones++;
}
bool Dron::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_r) {
		MessageType msg;
		msg.type = "stopMovement";
		sendEvent(msg, this);	//Manda un evento a las entidades que escuchan eventos
	}
	for (BrazoDron* br : brazosDron) {
		br->keyPressed(evt);
	}
	return true;
}
void Dron::frameRendered(const Ogre::FrameEvent& evt)
{
	//Activar movimiento automatico si se le permite moverse
	if (!stop) {
		if (myTimer->getMilliseconds() < timer) {
			mNode->translate(0, -420, 0, SceneNode::TS_LOCAL);
			mNode->roll(Degree(-1), SceneNode::TS_LOCAL);
			mNode->translate(0, 420, 0, SceneNode::TS_LOCAL);
			timer++;
		}
		else if (myTimer->getMilliseconds() < timer + 500) {
			mNode->yaw(Degree(dir), SceneNode::TS_LOCAL);
		}
		else {
			timer = myTimer->getMilliseconds() + 2000;
			dir = Ogre::Math::RangeRandom(0, 1) < 0.5f ? -3 : 3;
		}
		if (main_) {
			//Si no esta amarilla la esfera
			if (numdrones > 0)
				esfera->setMaterialName("IG2App/shRed");
			else
				esfera->setMaterialName("IG2App/shYellow");
		}
		else
			esfera->setMaterialName("IG2App/billboard");
	}
	else
		if(main_) esfera->setMaterialName("IG2App/shBlue");

	for (BrazoDron* b : brazosDron) {
		b->frameRendered(evt);
	}

}
void Dron::receiveEvent(MessageType msgType, EntidadIG* entidad)
{
	if (msgType.type == "stopMovement")
		stop = !stop;
	else if (msgType.type == "Hunt") {
		AxisAlignedBox aab1 = mNode->_getWorldAABB();
		AxisAlignedBox aab2 = entidad->mNode->_getWorldAABB();
		if (aab1.intersection(aab2) != AxisAlignedBox() && !main_) {
			numdrones--;
		}
	}
}
//-----------------------------------------------------------------------------------------

Avion::Avion(SceneNode* node) : EntidadIG(node)
{
	//Cuerpo
	cuerpoNode = mNode->createChildSceneNode();
	Entity* cuerpo = mSM->createEntity("sphere.mesh");
	cuerpo->setMaterialName("IG2App/shRed");
	cuerpoNode->setScale(1.2, 1.2, 1.2);
	cuerpoNode->attachObject(cuerpo);
	//Alas
	alaINode = mNode->createChildSceneNode();
	alaI = mSM->createEntity("cube.mesh");
	alaI->setMaterialName("IG2App/checkers");
	alaINode->setScale(2.5, 0.1, 1);
	alaINode->translate(180, 0, 0);
	alaINode->attachObject(alaI);

	alaDNode = mNode->createChildSceneNode();
	alaD = mSM->createEntity("cube.mesh");
	alaD->setMaterialName("IG2App/checkers");
	alaDNode->setScale(2.5, 0.1, 1);
	alaDNode->translate(-180, 0, 0);
	alaDNode->attachObject(alaD);
	//Frente
	frenteNode = mNode->createChildSceneNode();
	Entity* frente = mSM->createEntity("Barrel.mesh");
	frente->setMaterialName("IG2App/shOrange");
	frenteNode->setScale(12, 4, 12);
	frenteNode->translate(0, 0, 120);
	frenteNode->pitch(Degree(90));
	frenteNode->attachObject(frente);

	//Ninja
	pilotoNode = mNode->createChildSceneNode();
	Entity* ninja = mSM->createEntity("ninja.mesh");
	ninja->setMaterialName("IG2App/shYellow");
	pilotoNode->setScale(0.75, 0.75, 0.75);
	pilotoNode->translate(0, 10, 0);
	pilotoNode->yaw(Degree(180));
	pilotoNode->attachObject(ninja);
	//Helices
	heliceINode = mNode->createChildSceneNode();
	heliceI = new AspasMolino(heliceINode, false, true, 6);
	heliceINode->scale(0.1, 0.1, 0.1);
	heliceINode->translate(190, 0, 57);

	heliceDNode = mNode->createChildSceneNode();
	heliceD = new AspasMolino(heliceDNode, false, true, 6);
	heliceDNode->scale(0.1, 0.1, 0.1);
	heliceDNode->translate(-190, 0, 57);
	//Foco
	/*SceneNode* lightNode = mNode->createChildSceneNode();
	Light* luzFoco = mSM->createLight();
	luzFoco->setType(Ogre::Light::LT_SPOTLIGHT);
	luzFoco->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	luzFoco->setDirection(Ogre::Vector3(0, -1, 0));
	luzFoco->setSpotlightRange(Degree(5.0f), Degree(30.0f), 10.0f);
	lightNode->attachObject(luzFoco);*/
	//10points
	SceneNode* pointsNode = mNode->createChildSceneNode();
	BillboardSet* bbSet = mSM->createBillboardSet("points", 1);
	bbSet->setDefaultDimensions(100, 50);
	bbSet->setMaterialName("IG2App/10points");
	// Colocar el BillboardSet en el grafo de la escena
	pointsNode->attachObject(bbSet);
	// la posición del nodo será la del BillboardSet
	// Crear los elementos del conjunto: establecer su posición
	Billboard* bb = bbSet->createBillboard(Vector3(0, 0, -150));

	//--ParticleSystem
	//trail
	SceneNode* pSysNode = mNode->createChildSceneNode();
	ParticleSystem* pSys = mSM->createParticleSystem("psEstela", "IG2App/Trail");
	pSys->setEmitting(true);
	pSysNode->attachObject(pSys);
	//smoke
	SceneNode* pSys_SmokeNode = mNode->createChildSceneNode();
	pSys_Smoke = mSM->createParticleSystem("psBroken", "IG2App/SmokePlane");
	pSys_Smoke->setEmitting(false);
	pSys_SmokeNode->attachObject(pSys_Smoke);
	//--
	//Timer
	myTimer = new Ogre::Timer();
	timer = myTimer->getMilliseconds() + 2000;
	stop = false;
	addListener(this);

	dir = Ogre::Math::RangeRandom(0, 1) < 0.5f ? -2 : 2;
}

bool Avion::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	heliceI->keyPressed(evt);
	heliceD->keyPressed(evt);

	if (evt.keysym.sym == SDLK_h) {
		MessageType msg;
		msg.type = "Hunt";
		sendEvent(msg, this);
	}
	if (evt.keysym.sym == SDLK_r) {
		MessageType msg;
		msg.type = "Explote";
		sendEvent(msg, this);
	}
	return true;
}
void Avion::frameRendered(const Ogre::FrameEvent& evt)
{
	//Activar movimiento automatico si se le permite moverse
	/*if (!stop) {
		if (myTimer->getMilliseconds() < timer) {
			mNode->translate(0, -425, 0, SceneNode::TS_LOCAL);
			mNode->pitch(Degree(1), SceneNode::TS_LOCAL);
			mNode->translate(0, 425, 0, SceneNode::TS_LOCAL);
			timer++;
		}
		else if (myTimer->getMilliseconds() < timer + 500) {
			mNode->yaw(Degree(dir), SceneNode::TS_LOCAL);
		}
		else {
			timer = myTimer->getMilliseconds() + 2000;
			dir = Ogre::Math::RangeRandom(0, 1) < 0.5f ? -2 : 2;
		}
		alaD->setMaterialName("IG2App/checkers");
		alaI->setMaterialName("IG2App/checkers");
	}
	else {
		alaD->setMaterialName("IG2App/shRed");
		alaI->setMaterialName("IG2App/shRed");
	}*/

	mNode->translate(280, 0, 0, SceneNode::TS_LOCAL);
	mNode->yaw(Degree(0.5), SceneNode::TS_LOCAL);
	mNode->translate(-280, 0, 0, SceneNode::TS_LOCAL);

	//Rotacion de aspas
	heliceD->rotarAspas(15);
	heliceI->rotarAspas(15);
}
void Avion::receiveEvent(MessageType msgType, EntidadIG* entidad)
{
	if(msgType.type == "stopMovement")
		stop = !stop;

	if (msgType.type == "Explote")
		pSys_Smoke->setEmitting(true);
}
//-----------------------------------------------------------------------------------------

Plano::Plano(SceneNode* node) : EntidadIG(node) {
	/*MeshManager::getSingleton().createPlane("planoMalla",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(Vector3::UNIT_Y, 0),
		1000, 1000, 100, 80, true, 1, 1.0, 1.0, Vector3::UNIT_Z);
	Entity* malla = mSM->createEntity("planoMalla");
	mNode->attachObject(malla);*/
}

void Plano::setReflejo(Camera* c, int w, int h)
{
	mp = new MovablePlane(Vector3::UNIT_Y, 0);
	mNode->attachObject(mp);
	c->enableReflection(mp);
	c->enableCustomNearClipPlane(mp);

	TexturePtr rttRef = TextureManager::getSingleton().createManual("rttReflejo", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, w, h, 0, PF_R8G8B8, TU_RENDERTARGET);

	RenderTexture* renderTexture = rttRef->getBuffer()->getRenderTarget();
	Viewport* vpt = renderTexture->addViewport(c);
	vpt->setClearEveryFrame(true);
	vpt->setBackgroundColour(ColourValue::Black);

	TextureUnitState* tu = ent->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->createTextureUnitState("rttReflejo");
	tu->setColourOperation(LBO_ADD);
	tu->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	tu->setProjectiveTexturing(true, c);
	renderTexture->addListener(this);
}

void Plano::setEspejo(Camera* c, int w, int h)
{
	mp = new MovablePlane(Vector3::UNIT_Y, 0);
	mNode->attachObject(mp);
	c->enableReflection(mp);
	c->enableCustomNearClipPlane(mp);

	TexturePtr rttRef = TextureManager::getSingleton().createManual("rttEspejo", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, w, h, 0, PF_R8G8B8, TU_RENDERTARGET);

	RenderTexture* renderTexture = rttRef->getBuffer()->getRenderTarget();
	Viewport* vpt = renderTexture->addViewport(c);
	vpt->setClearEveryFrame(true);
	vpt->setBackgroundColour(ColourValue::Black);

	TextureUnitState* tu = ent->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->createTextureUnitState("rttEspejo");
	tu->setColourOperation(LBO_REPLACE);
	tu->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	tu->setProjectiveTexturing(true, c);
	renderTexture->addListener(this);
}

void Plano::setEspejoTecho(Camera* c, int w, int h)
{
	mp = new MovablePlane(Vector3::UNIT_Y, 0);
	mNode->attachObject(mp);
	c->enableReflection(mp);
	c->enableCustomNearClipPlane(mp);

	TexturePtr rttRef = TextureManager::getSingleton().createManual("rttEspejoTecho", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, w, h, 0, PF_R8G8B8, TU_RENDERTARGET);

	RenderTexture* renderTexture = rttRef->getBuffer()->getRenderTarget();
	Viewport* vpt = renderTexture->addViewport(c);
	vpt->setClearEveryFrame(true);
	vpt->setBackgroundColour(ColourValue::Black);

	TextureUnitState* tu = ent->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->createTextureUnitState("rttEspejoTecho");
	tu->setColourOperation(LBO_REPLACE);
	tu->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	tu->setProjectiveTexturing(true, c);
	renderTexture->addListener(this);
}

//-----------------------------------------------------------------------------------------

Sinbad::Sinbad(SceneNode* node) : EntidadIG(node)
{
	//Sinbad
	sinbadNode = mNode->createChildSceneNode();
	sinbad = mSM->createEntity("Sinbad.mesh");
	sinbadNode->attachObject(sinbad);

	//Animaciones
	animStateSet = sinbad->getAllAnimationStates();
	for (auto it = animStateSet->getAnimationStateIterator().begin(); it != animStateSet->getAnimationStateIterator().end(); ++it)
		animStatesID.push_back(it->first);
	//Idle
	animStateIdleTop = sinbad->getAnimationState("IdleTop");
	animStateIdleTop->setLoop(true);
	animStateIdleTop->setEnabled(false);
	animStateIdleBase = sinbad->getAnimationState("IdleBase");
	animStateIdleBase->setLoop(true);
	animStateIdleBase->setEnabled(false);
	//Run
	animStateTop = sinbad->getAnimationState("RunTop");
	animStateTop->setLoop(true);
	animStateBase = sinbad->getAnimationState("RunBase");
	animStateBase->setLoop(true);
	//Dance
	animStateDance = sinbad->getAnimationState("Dance");
	animStateDance->setLoop(true);

	//---------Custom
	Animation* animation = mSM->createAnimation("animRun", duracion);
	NodeAnimationTrack* track = animation->createNodeTrack(0);
	track->setAssociatedNode(mNode);

	Vector3 keyframePos(-480, 50,280); Vector3 src(0, 0, 1); // posición y orientación iniciales
	Real durPaso = duracion / 6.0; // uniformes
	TransformKeyFrame* kf; // 4 keyFrames: origen(0), abajo, arriba, origen(3)

	float scale = 12.0f;
	kf = track->createNodeKeyFrame(durPaso * 0); // Keyframe 1: origen
	kf->setScale(Vector3(scale));
	kf->setTranslate(keyframePos); // origen
	kf = track->createNodeKeyFrame(durPaso * 1); // Keyframe 2:
	kf->setScale(Vector3(scale));
	kf->setRotation(src.getRotationTo(Vector3(0, 0, -0.3)));
	keyframePos = Vector3(-160, 50, 93.4);
	kf->setTranslate(keyframePos);
	kf = track->createNodeKeyFrame(durPaso * 2); // Keyframe 3: 
	kf->setScale(Vector3(scale));
	kf->setRotation(src.getRotationTo(Vector3(0, 0, -1)));
	keyframePos = Vector3(160, 50, -93.4);
	kf->setTranslate(keyframePos);
	kf = track->createNodeKeyFrame(durPaso * 3); // Keyframe 4: mitad
	kf->setRotation(src.getRotationTo(Vector3(0, 0,-1))); 
	kf->setScale(Vector3(scale));
	keyframePos = Vector3(480, 50, -280);
	kf->setTranslate(keyframePos); // mitad
	kf = track->createNodeKeyFrame(durPaso * 4); // Keyframe 5: mitad
	kf->setScale(Vector3(scale));
	kf->setRotation(src.getRotationTo(Vector3(0, 0, 1)));
	keyframePos = Vector3(160, 50, -93.4);
	kf->setTranslate(keyframePos); 
	kf = track->createNodeKeyFrame(durPaso * 6); // Keyframe 6: origen
	kf->setRotation(src.getRotationTo(Vector3(0, 0, 1))); 
	kf->setScale(Vector3(scale));
	keyframePos = Vector3(-480, 50, 280);
	kf->setTranslate(keyframePos); // fin

	animStateRunAtoB = mSM->createAnimationState("animRun");
	animStateRunAtoB->setLoop(true);
	animStateRunAtoB->setEnabled(true);
	//---------

	equipArma2Manos();

	addListener(this);
}

void Sinbad::frameRendered(const Ogre::FrameEvent& evt)
{
	if (!dead) {
		if (!dance) {
			animStateDance->setEnabled(false);
			animStateBase->setEnabled(true);
			animStateTop->setEnabled(true);
			animStateBase->addTime(evt.timeSinceLastFrame);
			animStateTop->addTime(evt.timeSinceLastFrame);
		}
		else {
			animStateBase->setEnabled(false);
			animStateTop->setEnabled(false);
			animStateDance->setEnabled(true);
			animStateDance->addTime(evt.timeSinceLastFrame);
		}

		animStateRunAtoB->addTime(evt.timeSinceLastFrame);
	}
	else {
		//Muere
		animStateIdleTop->addTime(evt.timeSinceLastFrame);
		animStateIdleBase->addTime(evt.timeSinceLastFrame);
	}
}

bool Sinbad::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_c) {
		dance = !dance;
	}
	return true;
}

void Sinbad::receiveEvent(MessageType msgType, EntidadIG* entidad)
{
	if (msgType.type == "Explote") {
		dead = true;

		animStateBase->setEnabled(false);
		animStateTop->setEnabled(false);
		animStateDance->setEnabled(false);
		animStateIdleTop->setEnabled(true);
		animStateIdleBase->setEnabled(true);
		sinbadNode->yaw(Degree(90.0f), SceneNode::TS_PARENT);
		sinbadNode->roll(Degree(90.0f), SceneNode::TS_PARENT);
		sinbadNode->translate(0, -4, 0);

		MessageType msg;
		msg.type = "ExploteBomb";
		sendEvent(msg);
	}
}

void Sinbad::equipArma(bool state)
{
	if(state){
		//Crear arma
		if(sword == nullptr)
			sword = mSM->createEntity("Sword.mesh");

		if(rightHand)
			sinbad->attachObjectToBone("Handle.R", sword);
		else
			sinbad->attachObjectToBone("Handle.L", sword);
	}
	else {
		//Borrar arma/s
		if (sword != nullptr) {
			sinbad->detachObjectFromBone(sword);
			delete sword; sword = nullptr;
		}
		
		if (sword2 != nullptr) {
			sinbad->detachObjectFromBone(sword2);
			delete sword2; sword2 = nullptr;
		}
	}
}

void Sinbad::equipArma2Manos()
{
	//Crear armas
	if (sword == nullptr)
		equipArma(true);

	if (sword2 == nullptr)
		sword2 = mSM->createEntity("Sword.mesh");

	if(!rightHand)
		sinbad->attachObjectToBone("Handle.R", sword2);
	else
		sinbad->attachObjectToBone("Handle.L", sword2);
}

void Sinbad::switchArma()
{
	equipArma(false);
	rightHand = !rightHand;
	equipArma(true);
}

//-----------------------------------------------------------------------------------------

Bomba::Bomba(SceneNode* node) : EntidadIG(node)
{
	Entity* bomba = mSM->createEntity("Barrel.mesh");
	// 8
	//bomba->setMaterialName("Practica2/BombaTeselada");
	//12
	//bomba->setMaterialName("Practica2/Holes");
	//13
	bomba->setMaterialName("Practica2/HolesAndLighting");
	mNode->attachObject(bomba);
	//------------------
	Animation* animation = mSM->createAnimation("animBomba", duracion);
	NodeAnimationTrack* track = animation->createNodeTrack(0);
	track->setAssociatedNode(mNode);

	Vector3 keyframePos(0.0); Vector3 src(0, 0, 1); // posición y orientación iniciales
	Real durPaso = duracion / 4.0; // uniformes
	TransformKeyFrame* kf; // 4 keyFrames: origen(0), abajo, arriba, origen(3)
	kf = track->createNodeKeyFrame(durPaso * 0); // Keyframe 0: origen
	kf->setScale(Vector3(8));
	kf = track->createNodeKeyFrame(durPaso * 1); // Keyframe 1: abajo
	kf->setScale(Vector3(8));
	keyframePos += Ogre::Vector3::NEGATIVE_UNIT_Y * longDesplazamiento;
	kf->setTranslate(keyframePos); // Abajo
	kf->setRotation(src.getRotationTo(Vector3(1, 0, 1))); // Yaw(45)
	kf = track->createNodeKeyFrame(durPaso * 3); // Keyframe 2: arriba
	kf->setScale(Vector3(8));
	keyframePos += Ogre::Vector3::UNIT_Y * longDesplazamiento * 2;
	kf->setTranslate(keyframePos); // Arriba
	kf->setRotation(src.getRotationTo(Vector3(-1, 0, 1))); // Yaw(-45)
	kf = track->createNodeKeyFrame(durPaso * 4); // Keyframe 3: origen
	kf->setScale(Vector3(8));
	//------------------
	//--ParticleSystem
	SceneNode* pSysNode = mNode->createChildSceneNode();
	pSys = mSM->createParticleSystem("psSmoke", "IG2App/Smoke");
	pSys->setEmitting(false);
	pSysNode->attachObject(pSys);
	//--

	animState = mSM->createAnimationState("animBomba");
	animState->setLoop(true);
	animState->setEnabled(true);

	//Timer
	myTimer = new Ogre::Timer();
	addListener(this);
}

void Bomba::frameRendered(const Ogre::FrameEvent& evt)
{
	if(!stopAnim)
		animState->addTime(evt.timeSinceLastFrame);

	if (exploteBomb && !exploted) {
		if (myTimer->getMilliseconds() >= timer) {
			pSys->setEmitting(true);
			exploted = true;
		}
		else
			timer++;
	}
}

bool Bomba::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_t) {
		MessageType msg;
		msg.type = "stopRiver";
		sendEvent(msg);	//Manda un evento a las entidades que escuchan eventos
	}
	return true;
}

void Bomba::receiveEvent(MessageType msgType, EntidadIG* entidad)
{
	if (msgType.type == "stopRiver")
		stopAnim = !stopAnim;

	if (msgType.type == "ExploteBomb") {
		timer = myTimer->getMilliseconds() + 5000;
		exploteBomb = true;
	}

}

//-----------------------------------------------------------------------------------------

Rio::Rio(SceneNode* node) : Plano(node)
{
	MeshManager::getSingleton().createPlane("rioMalla",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(Vector3::UNIT_Y, 0),
		1200, 800, 120, 80, true, 1, 1.0, 1.0, Vector3::UNIT_Z);
	rio = mSM->createEntity("rioMalla");
	rio->setMaterialName("IG2App/water");
	ent = rio;
	mNode->attachObject(rio);

	//--Billboards
	/*SceneNode* nieblaNode = mNode->createChildSceneNode();
	BillboardSet* bbSet = mSM->createBillboardSet("niebla", 10);
	bbSet->setDefaultDimensions(800, 600);
	bbSet->setMaterialName("IG2App/smoke");
	nieblaNode->attachObject(bbSet);
	nieblaNode->translate(350, 200, -400);

	for (int i = 0; i < 10; i++)
		Billboard* bb = bbSet->createBillboard(Vector3(0, 0, (80 * i)));*/
	//--
	
	//Timer
	myTimer = new Ogre::Timer();
	stopped = false;
	addListener(this);
}

void Rio::frameRendered(const Ogre::FrameEvent& evt)
{
	//Secar rio
	if (stopRiver && !stopped) {
		if (myTimer->getMilliseconds() >= timer) {
			rio->setMaterialName("IG2App/rocks");
			stopped = true;
		}
		else
			timer++;
	}
}

void Rio::receiveEvent(MessageType msgType, EntidadIG* entidad)
{
	if (msgType.type == "stopRiver")
		stopRiver = !stopRiver;
	if (stopRiver) {
		timer = myTimer->getMilliseconds() + 5000;
	}
	else {
		rio->setMaterialName("IG2App/water");
		stopped = false;
	}
}

//-----------------------------------------------------------------------------------------

Plataforma::Plataforma(SceneNode* node, float width, float height, PLAT_TEXTURE tex_) : EntidadIG(node)
{
	Entity* plataforma = mSM->createEntity("cube.mesh");
	switch (tex_)
	{
	case Plataforma::RED:
			plataforma->setMaterialName("IG2App/Red");
		break;
	case Plataforma::YELLOW:
			plataforma->setMaterialName("IG2App/Yellow");
		break;
	}
	mNode->attachObject(plataforma);
	mNode->scale(width, 0.1, height);
}

Espejo::Espejo(SceneNode* node, std::string name) : Plano(node)
{
	MeshManager::getSingleton().createPlane("espejoMalla" + name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(Vector3::UNIT_Y, 0),
		1200, 800, 120, 80, true, 1, 1.0, 1.0, Vector3::UNIT_Z);
	esp = mSM->createEntity("espejoMalla" + name);
	ent = esp;
	mNode->attachObject(esp);
}
