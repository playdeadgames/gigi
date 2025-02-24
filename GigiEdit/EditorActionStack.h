#pragma once

#include <vector>
#include <string>


struct RenderGraph;

class EditorActionStack
{
public:
	void Push(const RenderGraph& renderGraph);
	bool Undo(RenderGraph& renderGraph);
	bool Redo(RenderGraph& renderGraph);
	void Clear();

private:
	void PushRedo(const RenderGraph& renderGraph);
	void PushUndo(const RenderGraph& renderGraph);

	std::vector<std::string> m_undoStack;
	std::vector<std::string> m_redoStack;
};