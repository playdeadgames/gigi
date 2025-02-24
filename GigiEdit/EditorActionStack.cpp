#include "EditorActionStack.h"

#include "Schemas/Types.h"
#include "Schemas/JSON.h"

void EditorActionStack::Push(const RenderGraph& renderGraph)
{
	m_redoStack.clear();
	PushUndo(renderGraph);
}

bool EditorActionStack::Undo(RenderGraph& renderGraph)
{
	if (!m_undoStack.empty())
	{
		PushRedo(renderGraph);

		RenderGraph tempGraph;
		ReadFromJSONBuffer(tempGraph, m_undoStack.back());
		
		renderGraph = tempGraph;

		m_undoStack.pop_back();

		return true;
	}

	return false;
}

bool EditorActionStack::Redo(RenderGraph& renderGraph)
{
	if (!m_redoStack.empty())
	{
		PushUndo(renderGraph);

		RenderGraph tempGraph;
		ReadFromJSONBuffer(tempGraph, m_redoStack.back());
		
		renderGraph = tempGraph;
		
		m_redoStack.pop_back();
		return true;
	}

	return false;
}

void EditorActionStack::Clear()
{
	m_undoStack.clear();
	m_redoStack.clear();
}

void EditorActionStack::PushRedo(const RenderGraph& renderGraph)
{
	std::string& bufferString = m_redoStack.emplace_back();
	WriteToJSONBuffer(renderGraph, bufferString, false);
}

void EditorActionStack::PushUndo(const RenderGraph& renderGraph)
{
	std::string& bufferString = m_undoStack.emplace_back();
	WriteToJSONBuffer(renderGraph, bufferString, false);
}
