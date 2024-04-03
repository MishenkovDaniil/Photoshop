# Разработка графического редактора ("фотошопа") с использованием методов объектно-ориентированного программирования

***Проект - лишь учебный прототип, сделанный в короткие сроки. Главной целью являлись усвоение принципов ООП и укрепление навыков индивидуальной и совместной работы над объемным проектом.***

![](/resources/imgs/example.png?raw=true "Пример работы фотошопа.")

## ООП 

Фотошоп разрабатывался как оконный менеджер, поэтому была составлена и реализована иерархия классов. Первый в иерархии - Widget - чисто виртуальный класс, который необходим для однообразной работы с различными объектами на экране, среди которых окно (Window), холст (Canvas), различные кнопки (Button, String Button, etc.). 

Для работы с холстом были добавлены различные инструменты (tools). Так как в реальной жизни невозможно продумать и реализовать сразу все требуемые от приложения возможности, то очевидна потребность в добавлении управляющих элементов динамически. Для этой цели для класса Tool и его наследников (конкретных инструментов, таких как кисть, заливка, текст и т.д.) был сделан единый интерфейс, а также стандартизирована функция их динамической загрузки, так что любой инструмент теперь можно реализовать извне, без изменения и перекомпиляции самого приложения. Аналогичные действия были проведены с фильтрами (инструменты для изменения тона,яркости, света и тому подобного).


