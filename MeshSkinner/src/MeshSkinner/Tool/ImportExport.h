#pragma once

#include "Application/Core.h"
#include "Tool.h"

class ImportExport : public Tool
{
public:
	ImportExport(const std::string &toolWindowName = "Import Export", Scene *scene = nullptr);
	virtual ~ImportExport() = default;

protected:
	virtual void OnUpdateUI() override;

private:
	Scene *scene = nullptr;

	std::string sourceFile;
	std::string targetFile;

	Ref<Material> weightPaintMaterial;
	Ref<Material> boneMaterial;
};
