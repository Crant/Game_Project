/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/
#pragma once

#include <AnimationStates.h>
#include <World/Observer.h>
#include <Graphics.h>
#include <string>
#include <map>

class ActorMovedEvent;
class Actor;


class ActorMovedEvent : public Event
{
public:
	Actor* zActor;
};

class ActorChangedModelEvent : public Event
{
public:
	Actor* zActor;
};

/*! Base class for Actors */
class Actor : public Observed
{
	std::string zModel;
	iMesh* zMesh;

	//Used For FBX
	std::map<std::string, iMesh*> zSubMeshes;
	unsigned int zID;
	Vector3 zPosition;
	Vector4 zRotation;

public:
	Actor(const unsigned int& ID);
	virtual ~Actor();

	inline const std::string& GetModel() const { return this->zModel; }
	inline iMesh* GetMesh() const { return this->zMesh; }
	inline const Vector3& GetScale() const { return this->zMesh->GetScaling(); }
	inline const Vector3& GetPosition() const { return this->zPosition; }
	inline const Vector4& GetRotation() const { return this->zMesh->GetRotationQuaternion(); }
	inline const unsigned int& GetID() const { return this->zID; }

	/*! Sets Actor Mesh data*/
	void SetModel(const std::string& model);
	
	// Set Actor Position
	void SetPosition(const Vector3& pos, const bool& updateMesh = true);

	// Set Scale
	void SetScale(const Vector3& scale);
	
	// Set Rotation
	void SetRotation(const Vector4& rot);

	/*! Sets the pointer to actor mesh */
	void SetMesh(iMesh* mesh);

	/*! Sets the Client Id given from the server*/
	void SetID(const int clientID);

	// FBX bound meshes
	void AddSubMesh(const std::string& model, iMesh* mesh);
	iMesh* GetSubMesh(const std::string& model);
	void RemoveSubMesh(const std::string& model);
};