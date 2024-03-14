#include <stdbool.h>
#include <stdlib.h>
#ifdef linux
#include <threads.h>
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#ifdef WIN32
#include "glexp.h"
#endif
#include "gtkgl.h"
#include "gtkvideo.h"
static unsigned int vao, program, yTexture, uTexture, vTexture;
extern AVFrame *frame;
static GtkWidget *glArea;
GThread *thr;
void update_gl(int signum)
{
    switch (signum)
    {
    #ifdef linux
    case SIGUSR1:
        gtk_gl_area_queue_render(GTK_GL_AREA(glArea));
        break;
    #endif
    #ifdef WIN32
    case SIGINT:
        //windows下只会捕获一次信号，因此需要再次捕获
        if (signal(SIGINT, update_gl) == SIG_ERR)
        {
            perror("signal failed");
            exit(EXIT_FAILURE);
        }
        gtk_gl_area_queue_render(GTK_GL_AREA(glArea));
        break;
    #endif
    default:
        break;
    }
    return;
}
void delete_resource(int status, void *params)
{
    if (program != 0)
        glDeleteProgram(program);
    glDeleteBuffers(1, (unsigned int*)params);
    if (frame != NULL)
    {
        av_frame_free(&frame);
        frame = NULL;
    }
    if (vao != 0)
        glDeleteVertexArrays(1, &vao);
    return;
}
unsigned int createShader(GLenum type, const char *fileName)
{
    char *dir = g_get_current_dir();
    char *path = alloca(strlen(dir) + strlen(fileName) + 1);
    strset(path, 0);
    strcat(path, dir);
    strcat(path, fileName);
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    fseek(fp, 0, SEEK_END);
    long leng = ftell(fp);
    char *data = alloca(leng * sizeof(char));
    rewind(fp);
    fread(data, 1, leng, fp);
    data[leng] = '\0';
    fclose(fp);
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char* const*)&data, NULL);
    glCompileShader(shader);
    int status, logLen;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == false) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char *log = alloca(logLen);
            glGetShaderInfoLog(shader, logLen, NULL, log);
            puts(log);
        }
        exit(EXIT_FAILURE);
    }
    return shader;
}
void on_realize(GtkGLArea *glArea)
{
    gtk_gl_area_make_current(glArea);
    if (gtk_gl_area_get_error(glArea) != NULL)
        return;
    #ifdef linux
    unsigned int vertShader = createShader(GL_VERTEX_SHADER, "/vert.glsl");
    unsigned int fragShader = createShader(GL_FRAGMENT_SHADER, "/frag.glsl");
    #endif
    #ifdef WIN32
    unsigned int vertShader = createShader(GL_VERTEX_SHADER, "\\vert.glsl");
    unsigned int fragShader = createShader(GL_FRAGMENT_SHADER, "\\frag.glsl");
    #endif
    unsigned int vbo = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    #ifdef linux
    on_exit(delete_resource, (void*)&vbo);
    #endif
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float vertex[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };
    unsigned int index[] = {
        0, 1, 3,
        1, 2, 3
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof vertex, vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(GL_FLOAT), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof index, index, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    int params;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    if (params == false) {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &params);
        if (params > 0) {
            char *log = alloca(params);
            glGetProgramInfoLog(program, params, NULL, log);
            puts(log);
        }
        return;
    }
    glDetachShader(program, vertShader);
    glDetachShader(program, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    glGenTextures(1, &yTexture);
    glGenTextures(1, &uTexture);
    glGenTextures(1, &vTexture);
    glUseProgram(program);
    #ifdef linux
    thrd_t thr;
    if (thrd_create(&thr, (thrd_start_t)play_video, NULL) == thrd_success)
        thrd_detach(thr);
    #endif
    #ifdef WIN32
    thr = g_thread_new(NULL, (GThreadFunc)play_video, NULL);
    #endif
    return;
}
static bool on_render(GtkGLArea *area, GdkGLContext *context)
{
    //glViewport(0, 0, gtk_widget_get_allocated_width(GTK_WIDGET(area)), gtk_widget_get_allocated_height(GTK_WIDGET(area)));
    glViewport(0, 0, gtk_widget_get_width(GTK_WIDGET(area)), gtk_widget_get_height(GTK_WIDGET(area)));
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (frame != NULL) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, yTexture);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frame->width, frame->height, 0, GL_RED, GL_UNSIGNED_BYTE, frame->data[0]);
        glUniform1i(glGetUniformLocation(program, "yTexture"), 0);
        glGenerateMipmap(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, uTexture);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frame->width / 2, frame->height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, frame->data[1]);
        glUniform1i(glGetUniformLocation(program, "uTexture"), 1);
        glGenerateMipmap(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, vTexture);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[2]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frame->width / 2, frame->height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, frame->data[2]);
        glUniform1i(glGetUniformLocation(program, "vTexture"), 2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glFlush();
    return true;
}
void on_activate(GtkApplication *app, gpointer data)
{
    #ifdef linux
    if (signal(SIGUSR1, update_gl) == SIG_ERR)
    {
        perror("signal failed");
        exit(EXIT_FAILURE);
    }
    #endif
    #ifdef WIN32
    if (signal(SIGINT, update_gl) == SIG_ERR)
    {
        perror("signal failed");
        exit(EXIT_FAILURE);
    }
    #endif
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    glArea = gtk_gl_area_new();
    gtk_window_set_child(GTK_WINDOW(window), glArea);
    g_signal_connect(glArea, "realize", G_CALLBACK(on_realize), NULL);
    g_signal_connect(glArea, "render", G_CALLBACK(on_render), NULL);
    gtk_widget_set_visible(window, true);
    return;
}
