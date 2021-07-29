# Простая программа, реализованная с помощью элементов интерфейса API Windows

## Шуточное окно с отсутствием возможности нажать на одну из кнопок

При открытии пользователем программы (\*.exe файла) появляется _окно_ с двумя _кнопками_ "Да" и "Нет" и _текстом_, провоцирующем на нажатие кнопки "Нет". 
При попытке нажатия на данную кнопку она начинает "бегать" по экрану. 

Пользователь не имеет возможность выйти из программы с помощью кнопки "Закрыть". 
Единственный вариант дальнейших действий - нажатие на кнопку "Да". Далее происходит смена текста и появление кнопки для выхода из программы.

<p align="center">
  <a href="https://wmpics.pics/pm-TCJW.html">
    <img src="https://wmpics.pics/dm-TCJW.png"/>
  </a>
  &nbsp;&nbsp;&nbsp; 
  <a href="https://wmpics.pics/pm-TCJW.html#image565493">
    <img src="https://wmpics.pics/dm-C3XX.png"/>
  </a>
</p>
  
<p align="center">
  <a href="https://wmpics.pics/pm-TCJW.html#image565494">
    <img src="https://wmpics.pics/dm-5I5D.png"/>
  </a>
  &nbsp;&nbsp;&nbsp; 
  <a href="https://wmpics.pics/pm-TCJW.html#image565495">
    <img src="https://wmpics.pics/dm-1G47.png"/>
  </a>
</p>

[Видео](https://drive.google.com/file/d/1lTG30_2S-FnCZBE2vAUjItj6CXrXPEFz/view?usp=sharing) с демонстрацией работы программы.

## Основные моменты при реализации

+ Программа реализована в среде _Microsoft Visual Studio 2019_. При клонировании репозитория в MS VS запуск программы необходимо осуществлять с помощью WinAPI_joke.sln. 
После запуска программы в папке WinAPI_joke/Debug будет создан файл WinAPI_joke.exe, работающий отдельно.

+ Файл _Source.cpp_ содержит основную программу: главную функцию, функцию обработки сообщений от окна и функцию перемещения кнопки. 

+ Для функционирования программы _координаты мыши_ определяются на экране монитора, далее переводятся в координаты рабочего окна (hwnd - дескриптор рабочего окна).

```c++
LPPOINT pPnt = (LPPOINT)malloc(sizeof(*pPnt));
...
GetCursorPos(pPnt);
ScreenToClient(hwnd, pPnt);
```

+ _Новая позиция кнопки_, при приближении к ней мыши, определяется с помощью подсчета. Скорость, то есть количество пикселей по диагонали при перемещении кнопки, задается константой. Кнопка движется в том же направлении, что и мышь.

```c++
int dx = x - xPos;
int dy = y - yPos;
float dxy = sqrt(dx * dx + dy * dy);
int kdx = speedButton * abs(dx) / (int)dxy;
int kdy = speedButton * abs(dy) / (int)dxy;

masButtonPos[0][0] = (dx <= 0) ? masButtonPos[0][0] - kdx : masButtonPos[0][0] + kdx;
masButtonPos[0][1] = (dy <= 0) ? masButtonPos[0][1] - kdy : masButtonPos[0][1] + kdy;
```
