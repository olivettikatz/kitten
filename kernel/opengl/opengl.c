#include "../backend.h"
#include <string.h>
#include <malloc.h>
#include <GL/glut.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <SOIL/SOIL.h>

int ids_init_opengl()
{
	char *argv[] = {NULL};
	int argc = 0;

	glutInit(&argc, argv);
	return 0;
}

int ids_halt_opengl()
{
	return 0;
}

void ids_opengl_render()
{
	if (ids_render)
	{
		printf("rendering... %ix%i\n", ids_size.x, ids_size.y);
		glClear(GL_COLOR_BUFFER_BIT);
		ids_render(ids_size, 0, 0);
		glutSwapBuffers();
	}
}

void ids_opengl_reshape(int w, int h)
{
	ids_size.x = w;
	ids_size.y = h;

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, w, h);
	glOrtho(-1, w+1, h+1, -1, -1, 2);

	if (ids_resize)
	{
		ids_resize(ids_size, 0, 0);
		ids_opengl_render();
	}
}

unsigned int ids_opengl_key_convert(unsigned char glk)
{
	unsigned int ik = glk;
	if (ik == 27)
		ik = IDS_KEY_ESCAPE;
	return ik;
}

unsigned int ids_opengl_special_convert(unsigned int k)
{
	unsigned int ik = 0;
	if (k == GLUT_KEY_F1)
		ik = IDS_KEY_F(1);
	else if (k == GLUT_KEY_F2)
		ik = IDS_KEY_F(2);
	else if (k == GLUT_KEY_F3)
		ik = IDS_KEY_F(3);
	else if (k == GLUT_KEY_F4)
		ik = IDS_KEY_F(4);
	else if (k == GLUT_KEY_F5)
		ik = IDS_KEY_F(5);
	else if (k == GLUT_KEY_F6)
		ik = IDS_KEY_F(6);
	else if (k == GLUT_KEY_F7)
		ik = IDS_KEY_F(7);
	else if (k == GLUT_KEY_F8)
		ik = IDS_KEY_F(8);
	else if (k == GLUT_KEY_F9)
		ik = IDS_KEY_F(9);
	else if (k == GLUT_KEY_F10)
		ik = IDS_KEY_F(10);
	else if (k == GLUT_KEY_F11)
		ik = IDS_KEY_F(11);
	else if (k == GLUT_KEY_F12)
		ik = IDS_KEY_F(12);
	else if (k == GLUT_KEY_LEFT)
		ik = IDS_KEY_LEFT;
	else if (k == GLUT_KEY_RIGHT)
		ik = IDS_KEY_RIGHT;
	else if (k == GLUT_KEY_UP)
		ik = IDS_KEY_UP;
	else if (k == GLUT_KEY_DOWN)
		ik = IDS_KEY_DOWN;
	else if (k == GLUT_KEY_PAGE_UP)
		ik = IDS_KEY_PAGEUP;
	else if (k == GLUT_KEY_PAGE_DOWN)
		ik = IDS_KEY_PAGEDOWN;
	else if (k == GLUT_KEY_HOME)
		ik = IDS_KEY_HOME;
	else if (k == GLUT_KEY_END)
		ik = IDS_KEY_END;
	else if (k == GLUT_KEY_INSERT)
		ik = IDS_KEY_INSERT;
	return ik;
}

unsigned int ids_opengl_mod_convert()
{
	unsigned int im = 0;
	int m = glutGetModifiers();

	if (m & GLUT_ACTIVE_CTRL)
		im |= IDS_MOD_CONTROL;
	if (m & GLUT_ACTIVE_SHIFT)
		im |= IDS_MOD_SHIFT;
	if (m & GLUT_ACTIVE_ALT)
		im |= IDS_MOD_ALT;

	return im;
}

void ids_opengl_keyboard(unsigned char k, int x, int y)
{
	if (ids_keydown)
		ids_keydown(ids_at(x, y), ids_opengl_key_convert(k), ids_opengl_mod_convert());
}

void ids_opengl_keyboard_up(unsigned char k, int x, int y)
{
	if (ids_keyup)
		ids_keyup(ids_at(x, y), ids_opengl_key_convert(k), ids_opengl_mod_convert());
}

void ids_opengl_special(int k, int x, int y)
{
	if (ids_keydown)
		ids_keydown(ids_at(x, y), ids_opengl_special_convert(k), ids_opengl_mod_convert());
}

void ids_opengl_special_up(int k, int x, int y)
{
	if (ids_keyup)
		ids_keyup(ids_at(x, y), ids_opengl_special_convert(k), ids_opengl_mod_convert());
}

void ids_opengl_mouse(int b, int s, int x, int y)
{
	if (ids_mousedown && s == GLUT_DOWN)
		ids_mousedown(ids_at(x, y), b, 0);
	if (ids_mousedown && s == GLUT_UP)
		ids_mouseup(ids_at(x, y), b, 0);
}

void ids_opengl_motion(int x, int y)
{
	ids_mouse.x = x;
	ids_mouse.y = y;

	if (ids_mousemove)
		ids_mousemove(ids_mouse, 0, 0);
}

void ids_opengl_passive_motion(int x, int y)
{
	ids_mouse.x = x;
	ids_mouse.y = y;

	if (ids_mousemove)
		ids_mousemove(ids_mouse, 0, 0);
}

int ids_main_opengl_lazy()
{
	glutInitWindowSize(ids_size.x, ids_size.y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("");

	glutDisplayFunc(ids_opengl_render);
	glutReshapeFunc(ids_opengl_reshape);
	glutKeyboardFunc(ids_opengl_keyboard);
	glutKeyboardUpFunc(ids_opengl_keyboard_up);
	glutSpecialFunc(ids_opengl_special);
	glutSpecialUpFunc(ids_opengl_special_up);
	glutMouseFunc(ids_opengl_mouse);
	glutMotionFunc(ids_opengl_motion);
	glutPassiveMotionFunc(ids_opengl_passive_motion);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glDisable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}

int ids_main_opengl_fps()
{
	glutIdleFunc(ids_opengl_render);
	return ids_main_opengl_lazy();
}

int ids_title_opengl(char *t)
{
	glutSetWindowTitle(t);
	return 0;
}

int ids_request_render_opengl()
{
	ids_opengl_render();
	return 0;
}

int ids_color_opengl(char *color)
{
	char buf[3];
	buf[2] = 0;

	buf[0] = color[1];
	buf[1] = color[2];
	long r = strtol(buf, NULL, 16);

	buf[0] = color[3];
	buf[1] = color[4];
	long g = strtol(buf, NULL, 16);

	buf[0] = color[5];
	buf[1] = color[6];
	long b = strtol(buf, NULL, 16);

	glColor3f((float)r/(float)0xff, (float)g/(float)0xff, (float)b/(float)0xff);
	return 0;
}

#define IDS_OPENGL_X(c) (float)c.x
#define IDS_OPENGL_Y(c) (float)c.y

int ids_render_pixel_opengl(ids_coord loc)
{
	glPointSize(1.0f);
	glBegin(GL_POINTS);
	glVertex2f(IDS_OPENGL_X(loc), IDS_OPENGL_Y(loc));
	glEnd();
	return 0;
}

int ids_render_line_opengl(ids_coord start, ids_coord end)
{
	glBegin(GL_LINES);
	glVertex2f(IDS_OPENGL_X(start), IDS_OPENGL_Y(start));
	glVertex2f(IDS_OPENGL_X(end), IDS_OPENGL_Y(end));
	glEnd();
	return 0;
}

int ids_render_rect_opengl(ids_coord topleft, ids_coord bottomright)
{
	glBegin(GL_QUADS);
	glVertex2f(IDS_OPENGL_X(topleft), IDS_OPENGL_Y(topleft));
	glVertex2f(IDS_OPENGL_X(bottomright), IDS_OPENGL_Y(topleft));
	glVertex2f(IDS_OPENGL_X(bottomright), IDS_OPENGL_Y(bottomright));
	glVertex2f(IDS_OPENGL_X(topleft), IDS_OPENGL_Y(bottomright));
	glEnd();
	return 0;
}

char **ids_fontlist_opengl()
{
	return NULL;
}

float ids_font_height;
GLuint *ids_font_textures;
GLuint ids_font_list;

int ids_next_p2(int n)
{
	int r = 1;
	while(r < n)
		r <<= 1;
	return r;
}

void ids_font_make_list(FT_Face face, char ch, GLuint base, GLuint *textures)
{
	if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
		IDS_ERROR_INIT("cannot load glyph");

	FT_Glyph glyph;
	if (FT_Get_Glyph(face->glyph, &glyph))
		IDS_ERROR_INIT("cannot get glyph");

	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	FT_Bitmap bitmap = bitmap_glyph->bitmap;

	int w = ids_next_p2(bitmap.width);
	int h = ids_next_p2(bitmap.rows);

	GLubyte *edata = (GLubyte *)malloc(sizeof(GLubyte)*2*w*h);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			edata[2*(i+j*w)] = edata[2*(i+j*w)+1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i+bitmap.width*j];
		}
	}

	glBindTexture(GL_TEXTURE_2D, textures[ch]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, edata);
	free(edata);

	glNewList(base+ch, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, textures[ch]);
	glPushMatrix();
	glTranslatef(bitmap_glyph->left, 0, 0);
	glTranslatef(0, bitmap_glyph->top-bitmap.rows, 0);

	float x = (float)bitmap.width/(float)w;
	float y = (float)bitmap.rows/(float)h;

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(0, bitmap.rows);
	glTexCoord2d(0, y); glVertex2f(0, 0);
	glTexCoord2d(x, y); glVertex2f(bitmap.width, 0);
	glTexCoord2d(x, 0); glVertex2f(bitmap.width, bitmap.rows);
	glEnd();
	glPopMatrix();
	glTranslatef(face->glyph->advance.x >> 6, 0, 0);
	glEndList();
}

int ids_font_opengl_base(char *f, unsigned int h)
{
	ids_font_textures = (GLuint *)malloc(sizeof(GLuint)*128);
	ids_font_height = (float)h;
	FT_Library library;
	if (FT_Init_FreeType(&library))
	{
		IDS_ERROR_INIT("cannot init freetype");
		return 1;
	}

	FT_Face face;
	if (FT_New_Face(library, f, 0, &face))
	{
		IDS_ERROR_INIT("cannot init face");
		return 2;
	}

	FT_Set_Char_Size(face, h<<6, h<<6, 96, 96);
	ids_font_list = glGenLists(128);
	glGenTextures(128, ids_font_textures);

	for (unsigned char i = 0; i < 128; i++)
		ids_font_make_list(face, i, ids_font_list, ids_font_textures);

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return 0;
}

int ids_font_opengl(char *f, int size)
{
	if (f == NULL)
		return 1;

	return ids_font_opengl_base(f, size);
}

int ids_render_text_opengl(ids_coord topleft, char *str)
{
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
	glPopAttrib();

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glListBase(ids_font_list);

	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(IDS_OPENGL_X(topleft), IDS_OPENGL_Y(ids_size)-IDS_OPENGL_Y(topleft), 0);
	glMultMatrixf(modelview);
	glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
	glPopMatrix();

	glPopAttrib();

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();

	return 0;
}

ids_bitmap ids_load_bitmap_opengl(char *path)
{
	ids_bitmap rtn;
	rtn.size = ids_at(0, 0);
	rtn.data = NULL;

	GLuint tex = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (tex == 0)
		return rtn;

	rtn.data = (void *)tex;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, &rtn.size.x);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_HEIGHT, &rtn.size.y);

	return rtn;
}

int ids_render_bitmap_opengl(ids_coord topleft, ids_bitmap b)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, (GLuint)b.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(IDS_OPENGL_X(topleft), IDS_OPENGL_Y(topleft));
	glTexCoord2f(0, 1); glVertex2f(IDS_OPENGL_X(topleft), IDS_OPENGL_Y(topleft)+IDS_OPENGL_Y(b.size));
	glTexCoord2f(1, 1); glVertex2f(IDS_OPENGL_X(topleft)+IDS_OPENGL_X(b.size), IDS_OPENGL_Y(topleft)+IDS_OPENGL_Y(b.size));
	glTexCoord2f(1, 0); glVertex2f(IDS_OPENGL_X(topleft)+IDS_OPENGL_X(b.size), IDS_OPENGL_Y(topleft));
	glEnd();

	return 0;
}

int ids_destroy_bitmap_opengl(ids_bitmap b)
{
	GLuint tmp = (GLuint)b.data;
	glDeleteTextures(1, &tmp);
	return 0;
}

