#include "trajscene.hpp"

TrajBuffer::TrajBuffer(Traj *traj)
    : m_traj(traj)
{
    initializeOpenGLFunctions();

    /* Create VAO with VBO */
    m_vao.create();
    m_vao.bind();
        m_vbo.create();
        m_vbo.bind();
        m_vbo.allocate(traj->getConstData(), traj->getCount() * sizeof(GLfloat));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
    m_vao.release();
}

TrajBuffer::~TrajBuffer()
{
    delete m_traj;

    m_vbo.destroy();
    m_vao.destroy();
}

void TrajBuffer::bind()
{
    m_vao.bind();
}

void TrajBuffer::release()
{
    m_vao.release();
}

Traj *TrajBuffer::getTraj() const
{
    return m_traj;
}

QVector3D TrajBuffer::getColor() const
{
    QColor color = m_traj->getColor();
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

int TrajBuffer::getVertexCount() const
{
    return m_traj->getVertexCount();
}

int TrajBuffer::getVertexCount(double time) const
{
    return m_traj->getVertexCount(time);
}

bool TrajBuffer::getDisplayStatus() const
{
    return m_traj->isDisplayed();
}




TrajScene::TrajScene(QWidget *parent)
    : QOpenGLWidget(parent)
{
    /* Set vsync */
    QSurfaceFormat fmt;
    fmt.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(fmt);
}

TrajScene::~TrajScene()
{
    for (int i = 0; i < m_buffers.count(); i++) {
        delete m_buffers[i];
    }
}

void TrajScene::addTraj(Traj *traj)
{
    makeCurrent();
    m_buffers.push_back(new TrajBuffer(traj));
    update();
}

void TrajScene::deleteTraj(int pos)
{
    m_buffers.removeAt(pos);
    update();
}

void TrajScene::focusTraj(Traj *traj)
{
    m_camera.moveCenter(traj->getAverage());
    update();
}

void TrajScene::setCurrentTime(double time)
{
    m_currentTime = time;
    update();
}

void TrajScene::setBackgroundColor(const QColor &color)
{
    makeCurrent();

    glClearColor(color.redF(), color.greenF(), color.blueF(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update();
}

void TrajScene::initializeGL()
{
    initializeOpenGLFunctions();
    setBackgroundColor(Qt::gray);

    /* Create shader program */
    m_shprogram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl");
    m_shprogram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl");
    m_shprogram.bindAttributeLocation("position", 0);
    m_shprogram.link();

    /* Get locations */
    m_shprogram.bind();
        m_transformLoc = m_shprogram.uniformLocation("transform");
        m_colorLoc = m_shprogram.uniformLocation("color");
    m_shprogram.release();
}

void TrajScene::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(YFOV, float(w) / h, ZNEAR, ZFAR);
}

void TrajScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    /* Configure scene matrixes */
    m_model.setToIdentity();

    /* Draw scene */
    m_shprogram.bind();
    m_shprogram.setUniformValue(m_transformLoc, m_proj * m_camera.getViewMatrix() *  m_model);
        for (int i = 0; i < m_buffers.count(); i++) {
            if (m_buffers[i]->getDisplayStatus()) {
                m_buffers[i]->bind();
                    m_shprogram.setUniformValue(m_colorLoc, m_buffers[i]->getColor());
                    //qDebug() << m_buffers[i]->getVertexCount(m_currentTime);
                    glDrawArrays(GL_QUADS, 0, m_buffers[i]->getVertexCount(m_currentTime));
                m_buffers[i]->release();
            }
        }

    m_shprogram.release();
}

void TrajScene::mousePressEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
}

void TrajScene::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::RightButton)) {
        float dyaw = YAW_SENS * (m_mousePos.x() - event->pos().x());
        float dpitch = PITCH_SENS * (event->pos().y() - m_mousePos.y());
        m_camera.rotate(dyaw, dpitch);

        m_mousePos = event->pos();

        update();
    }
}

void TrajScene::wheelEvent(QWheelEvent *event)
{
    int numSteps = event->delta();

    if (numSteps > 0) {
        m_camera.zoom(ZOOMUP_FACTOR);
    }
    else {
        m_camera.zoom(ZOOMDOWN_FACTOR);
    }

    update();
}
