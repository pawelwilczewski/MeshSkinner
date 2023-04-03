#pragma once

#include "Tool.h"

class ImportExportTool : public Tool
{
public:
	ImportExportTool(const std::string &toolWindowName = "Import Export", Scene *scene = nullptr);
	virtual ~ImportExportTool() = default;

protected:
	virtual void OnUpdateUI() override;

private:
	Scene *scene = nullptr;

	std::string sourceFile;
	std::string targetFile;

	Ref<Material> weightPaintMaterial;
	Ref<Material> boneMaterial;
};
