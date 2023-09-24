#pragma once
#include "EntidadIG.h"

using namespace Ogre;

class Aspa : public EntidadIG
{
public:
	Aspa(SceneNode* node, bool _con_Adorno = true);
	~Aspa() {};
};
//-----------------------------------------------------------------------------------------

class AspasMolino : public EntidadIG
{
public:
	AspasMolino(SceneNode* node, bool _conAdorno = true, bool sent_horario = true, int aspas = 6);
	~AspasMolino() {};
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	void rotarAspas(int grad);

protected:

	int numAspas;
	bool sent_horario;
	SceneNode* centroNode = nullptr;
	std::vector<Aspa*> aspas;
};
//-----------------------------------------------------------------------------------------

class Molino : public EntidadIG
{
public:
	Molino(SceneNode* node);
	~Molino() {};
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
protected:

	AspasMolino* aspasMolino = nullptr;
	SceneNode* aspasNode = nullptr;
	SceneNode* torreNode = nullptr;
	SceneNode* esferaNode = nullptr;
};
//-----------------------------------------------------------------------------------------

class RotorDron : public EntidadIG
{
public:
	RotorDron(SceneNode* node, bool sent_hor = true, int asp = 6);
	~RotorDron() {};
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual void frameRendered(const Ogre::FrameEvent& evt);
protected:

	int aspas;
	AspasMolino* aspasMolino = nullptr;
	SceneNode* esferaNode = nullptr;
	SceneNode* helicesNode = nullptr;
};
//-----------------------------------------------------------------------------------------

class BrazoDron : public EntidadIG
{
public:
	BrazoDron(SceneNode* node, bool sent_hor = true, int asp = 6);
	~BrazoDron() {};
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual void frameRendered(const Ogre::FrameEvent& evt);
protected:

	int aspas;
	RotorDron* rotorDron = nullptr;
	SceneNode* cilindroNode = nullptr;
	SceneNode* rotorNode = nullptr;
};
//-----------------------------------------------------------------------------------------

class Dron : public EntidadIG
{
public:
	Dron(SceneNode* node, bool main = true, int br = 8, int asp = 6);
	~Dron() {};
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual void frameRendered(const Ogre::FrameEvent& evt);
	virtual void receiveEvent(MessageType msgType, EntidadIG* entidad);
protected:
	static int numdrones;
	int timer, dir;
	Ogre::Timer* myTimer;

	int brazos, aspas;
	SceneNode* centroNode = nullptr;

	Entity* esfera = nullptr;
	std::vector<BrazoDron*> brazosDron;
	bool stop, main_;
};
//-----------------------------------------------------------------------------------------

class Avion : public EntidadIG
{
public:
	Avion(SceneNode* node);
	~Avion() {};
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual void frameRendered(const Ogre::FrameEvent& evt);
	virtual void receiveEvent(MessageType msgType, EntidadIG* entidad);
protected:
	
	int timer, dir;
	bool stop;
	Ogre::Timer* myTimer;

	ParticleSystem* pSys_Smoke;
	SceneNode* cuerpoNode = nullptr;
	SceneNode* alaINode = nullptr;
	SceneNode* alaDNode = nullptr;
	SceneNode* frenteNode = nullptr;
	SceneNode* pilotoNode = nullptr;
	SceneNode* heliceINode = nullptr;
	SceneNode* heliceDNode = nullptr;

	AspasMolino* heliceI = nullptr;
	AspasMolino* heliceD = nullptr;


	Entity* alaI = nullptr, * alaD = nullptr;
};
//-----------------------------------------------------------------------------------------

class Plano : public EntidadIG, Viewport::Listener, RenderTargetListener
{
protected:
	MovablePlane* mp;
	Entity* ent = nullptr;
public:
	Plano(SceneNode* node);
	~Plano() {};
	void setReflejo(Camera* c, int w, int h);
	void setEspejo(Camera* c, int w, int h);
	void setEspejoTecho(Camera* c, int w, int h);
};
//-----------------------------------------------------------------------------------------

class Sinbad : public EntidadIG {
protected:
	float duracion = 8.0f;
	AnimationState* animStateTop, *animStateBase, * animStateDance, * animStateIdleTop, *animStateIdleBase;
	AnimationState* animStateRunAtoB;

	AnimationStateSet* animStateSet;
	std::vector<std::string>animStatesID;

	SceneNode* sinbadNode;
	Entity* sinbad = nullptr;
	Entity* sword = nullptr;
	Entity* sword2 = nullptr;

	bool rightHand = true;
	bool dance = false, dead = false;

public:
	Sinbad(SceneNode* node);
	~Sinbad() {}
	virtual void frameRendered(const Ogre::FrameEvent& evt);
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual void receiveEvent(MessageType msgType, EntidadIG* entidad);

	void equipArma(bool state);
	void equipArma2Manos();
	void switchArma();
};
//-----------------------------------------------------------------------------------------

class Bomba : public EntidadIG {
protected:
	int timer;
	Ogre::Timer* myTimer;
	float duracion = 10.0f;
	float longDesplazamiento = 8.0f;
	bool stopAnim = false, exploteBomb = false, exploted = false;
	AnimationState* animState;
	ParticleSystem* pSys;
public:
	Bomba(SceneNode* node);
	~Bomba() {}
	virtual void frameRendered(const Ogre::FrameEvent& evt);
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
	virtual void receiveEvent(MessageType msgType, EntidadIG* entidad);
};
//-----------------------------------------------------------------------------------------

class Rio : public Plano
{
protected:
	int timer;
	bool stopRiver = false, stopped = false;
	Ogre::Timer* myTimer;
	Entity* rio = nullptr;
public:
	Rio(SceneNode* node);
	~Rio() {};
	virtual void frameRendered(const Ogre::FrameEvent& evt);
	virtual void receiveEvent(MessageType msgType, EntidadIG* entidad);
};

class Espejo : public Plano
{
protected:
	Entity* esp = nullptr;
public:
	Espejo(SceneNode* node, std::string name);
	~Espejo() {}
};
//-----------------------------------------------------------------------------------------

class Plataforma : public EntidadIG
{
public:
	enum PLAT_TEXTURE{
		RED, YELLOW
	};
	Plataforma(SceneNode* node, float width, float height, PLAT_TEXTURE tex_);
	~Plataforma() {};
};