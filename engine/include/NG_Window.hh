#ifndef _NGWINDOW_H_
#define _NGWINDOW_H_

#include <memory>

#include <glm/glm.hpp>

#include "NG_GL.hh"

namespace NG{
  class Window{
  public:
    Window(int width, int height, const char* name, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);
    ~Window();

    void Activate();
    void SwapBuffers();

    void SetInputMode(int mode, int value);
    void SetCursorPos(int x, int y);
    void CenterCursor();
    bool IsKeyPressed(int key) const;
    glm::vec2 GetCursorPos() const;
    glm::vec2 GetCenter() const;
    bool ShouldClose() const;

		void ScrollCallback(double xoffset, double yoffset);
		glm::vec2 GetScrollDistance() const;
		void ResetScrollDistance();

		void FocusCallback(int focused);
		bool IsFocused() const;
  private:
    int m_width;
    int m_height;
    const char* m_name;
    GLFWwindow* m_window;

		glm::vec2 m_scrolled;
		bool m_focused;

    std::shared_ptr<NG::GLFW> m_glfw;
    std::shared_ptr<NG::GLEW> m_glew;
  };
}

#endif /* _NGWINDOW_H_ */
