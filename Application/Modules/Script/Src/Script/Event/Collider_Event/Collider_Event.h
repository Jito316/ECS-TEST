#pragma once
class IGameObject;
struct CollisionResult;

//�������鑤�̃C�x���g
class Collider_Event
{
public:
	virtual void OnTrrigerEnter(const CollisionResult*/*_collision*/) {};
	virtual void OnTrrigerStay(const CollisionResult*/*_collision*/) {};
	virtual void OnCollisionEnter(const CollisionResult*/*_collision*/) {};
	virtual void OnCollisionStay(const CollisionResult*/*_collision*/) {};

	virtual void OnExit(const CollisionResult* /*_hitObj*/) {};
};