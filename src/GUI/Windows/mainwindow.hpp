#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../Boxes/doublevalueslidebox.hpp"
#include "../Panels/trajpanel.hpp"
#include "../../Opengl/trajscene.hpp"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:


private:
    QVBoxLayout *m_panelLayout;
    QVBoxLayout *m_sceneLayout;
    QHBoxLayout *m_mainLayout;
    TrajScene *m_scene;
    DoubleValueSlideBox *m_timeBox;
    TrajPanel *m_trajPanel;

signals:


};

#endif // MAINWINDOW_HPP