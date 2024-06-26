# ConsoleGUI

- ## 文件简介

  - ## consolegui.hpp

    `#include "consolegui.hpp"`

- ## 内置类的详细介绍

  - 看不懂可以直接跳过哦，这一节的内容没那么重要，不理解也没关系。

  ```c++
  struct Color;
  
  short Default;		// 默认颜色组合
  short Highlight;	// 文本高亮颜色组合
  short lastColor;	// 用于实现切换颜色组合，属于辅助变量
  
  Color(short Default=BlackWhite, short Highlight=BlackCyan);		// 构造函数
  void setDefault();		// 将文本颜色设置为Default组合
  void setHighlight();	// 将文本颜色设置为Highlight组合
  ```

  ```c++
  struct Parameter;
  
  DWORD buttonState;		// 鼠标按键记录
  DWORD controlKeyState;	// 控制按键状态
  DWORD eventFlag;		// 鼠标事件记录
  Menu *menu;				// Menu型指针，传入原菜单
  
  Parameter(MOUSE_EVENT_RECORD mouseEvent, Menu *menu);	// 构造函数
  ```

  ```c++
  struct Text;
  
  std::string text;			// 文本内容，会显示在控制台上
  Color color;			// 颜色属性，详见Color类
  COORD position;			// 文本位置
  callback function;		// 回调函数，详见Parameter类
  
  Text(std::string text, Color color, callback function);		// 构造函数
  bool operator==(const COORD&mousePosition);		// 重载运算符==，判断鼠标坐标是否与文本位置重合
  bool operator!=(const COORD&mousePosition);		// 重载运算符==，判断鼠标坐标是否不与文本位置重合
  ```

  ```c++
  class Menu;
  
  private:
  long sleepTime;		// 循环中等待时间，类似于帧率。值越小CPU占用率越高、画面越流畅。一般为50
  short consoleHeight, consoleWidth;		// 控制台长、宽属性，一般仅用于clearScreen函数
  vector<Text> lineData;		// Text型vector数组。存放菜单每行的信息，详见Text类
  
  protected:
  void GetConsoleSize();		// 获取控制台大小
  void clearScreen();		// 清屏，可以用system("cls");代替
  void write(std::string text, bool isEndl=false);		// 输出，可以选择传入第二个参数表示是否要换行
  void rewrite(Text data);		// 整行覆盖输出
  void initializePosition();			// 输出菜单，并对lineData内position成员变量初始化
  void refresh(COORD hangPosition);		// 刷新菜单，用于更新菜单中高亮文字
  bool implement(MOUSE_EVENT_RECORD mouseEvent);		// 自动调用回调函数
  
  public:
  Menu&push_back();		// 在菜单尾部添加一个换行
  Menu&push_back(std::string text);		// 在菜单尾部添加文本（不可点击）
  Menu&push_back(std::string text, callback function);	// 在菜单尾部添加按钮，按下时执行function();
  Menu&push_back(std::string text, callback function, short colorDefault, short colorHighlight);
  										// 在菜单尾部添加按钮并设置颜色，按下时执行function();
  Menu&pop_back();	// 从菜单尾部弹出一行
  Menu&clear();		// 清空菜单
  void showMenu();	// 显示菜单
  ```

- ## 使用方法详解

  - ### Menu类使用方法

    - #### 定义一个Menu型变量

      `Menu loop;`

      $loop$ 可以改成任意名称，下文中的 $loop$ 同理

      

    - #### 在目录末尾添加文本

      `loop.push_back("Title");`

      $Title$ 可以改成任意文本，它会显示在控制台上，但不能交互

      

    - #### 在目录末尾添加按钮

      `loop.push_back("Button",(callback)function);`

      $Button$ 可以改成任意文本，它会显示在控制台上，并且有对鼠标的交互（如文本高亮、点击交互等）。$function$ 需要改成自己实现的回调函数。$function$ 会在鼠标点击时被调用。$callback$ 是函数指针的类型，不理解不重要，大概知道它限制了回调函数的格式就行了。回调函数的格式参见后文对回调函数的专门讲解。

      

    - #### 在目录末尾添加按钮并设置颜色

      `loop.push_back("Button",(callback)function, BlackWhite, BlackCyan);`

      $Button$ 和 $function$ 属性同上。$BlackWhite$、$BlackCyan$ 可以替换为其它颜色。现在已定义：

      | 名称         | 背景色 | 文本色 |
  | :----------- | ------ | ------ |
      | $BlackWhite$ | 黑色   | 白色   |
      | $BlackCyan$  | 黑色   | 青色   |
    | $YellowBlue$ | 黄色   | 蓝色   |
    
    更多请参阅[微软官方文档](https://learn.microsoft.com/zh-cn/windows/console/console-screen-buffers#character-attributes)（https://learn.microsoft.com/zh-cn/windows/console/console-screen-buffers#character-attributes）

    
    
    - #### 从目录末尾删除元素
    
      `loop.pop_back();`
    
      删去最后一行
    
      
    
    - #### 清空目录
    
      `loop.clear();`
    
      删去当前目录中的所有元素
    
      
    
    - #### 显示目录
    
  - ### 回调函数  $(callback)function$ 详解

    - ### 格式&约定

      `bool function(Parameter data);`

      1. $function$ 可以是任意合法的函数名，$data$ 可以是任意合法的变量名

      2. 函数的返回类型必须是 $bool$。若返回 $false$ 表示继续显示菜单；返回 $true$ 表示退出菜单界面，退出 `void showMenu()`函数

      3. 函数参数必须仅有一个，且必须为 $Parameter$ 类

      4. `void showMenu()`函数在调用 $function$ 前会自动清屏，不需要手动处理

      5. 有关如何使用回调函数 $(callback)function$ ，可参见上文  “Menu类使用方法”  中  “在目录末尾添加按钮”  方法。这里给出示例：`loop.push_back("Button", function);`（注意，函数名 $function$ 后不带括号，具体格式可以自行查询有关  “c++函数指针”  的语法）

         

    - ### 函数参数 $Parameter$ 类详解

      ——*如何通过回调函数参数获取必要的信息*

      - #### 成员变量

        - ##### $buttonState$ 变量

          记录鼠标按键行为，包括：

          | $MouseLeftButton$   | 鼠标左键 |
          | ------------------- | -------- |
          | $MouseMiddleButton$ | 鼠标中键 |
          | $MouseRightButton$  | 鼠标右键 |

          指示了当前点击事件被哪个按键触发

          

        - ##### $controlKeyState$ 变量

          记录了鼠标点击时按下了哪些辅助键。

          具体可以参阅[微软官方文档](https://learn.microsoft.com/zh-cn/windows/console/mouse-event-record-str)（https://learn.microsoft.com/zh-cn/windows/console/mouse-event-record-str）

          

        - ##### $eventFlag$ 变量

          记录点击时鼠标事件状态，包括：

          | $MouseClick$       | 鼠标单击 |
          | ------------------ | -------- |
          | $MouseDoubleClick$ | 鼠标双击 |

          ~~其实还有鼠标滚轮事件，但是考虑到比较复杂，又是为了防止误触，Tptr决定先屏蔽鼠标滚轮事件~~

          

        - ##### $menu$ 变量

          `Menu* menu;`

          它就是指向调用它的菜单的指针！！

          可以这么用：

          ```c++
          bool function(Parameter data){
              data.menu->push_back("I am from a callback fuction!");
              return false;
          }
          ```

          这样，结合上文所述  “Menu类使用方法”  就可以在回调函数里轻松地向原菜单末尾添加一段新文本：I am from a call back function!

          开放这个指针是为了提高回调函数自由度。

          

      - #### 回调函数类型

        - `typdef bool(*callback)(Parameter);`

          callback作为一个定义好的函数指针类型，可直接使用

