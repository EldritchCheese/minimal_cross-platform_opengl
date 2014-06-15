#ifndef _NGWINDOW_H_
#define _NGWINDOW_H_

#include <glm/glm.hpp>
class GLFWwindow;
class GLFWmonitor;

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
    bool IsKeyPressed(int key);
    glm::vec2 GetCursorPos();

    glm::vec2 GetCenter();

    bool ShouldClose();
  private:
    int m_width;
    int m_height;
    const char* m_name;
    GLFWwindow* m_window;
  };
}

#endif /* _NGWINDOW_H_ */
