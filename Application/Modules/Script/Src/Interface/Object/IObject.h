#pragma once

class SCRIPT_API IObject
{
public:
	IObject() = default;
	virtual ~IObject() = default;

	virtual bool GetEnable() const = 0;
	virtual bool& WorkEnable() = 0;
	virtual void SetEnable(bool _active) = 0;

	virtual void Destroy() = 0;
	virtual bool GetDestroy() const = 0;
};
