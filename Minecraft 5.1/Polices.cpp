#include "Polices.h"

Polices::Polices() : textShader("polices.vert", "polices.frag"), textVBO(sizeof(float) * 6 * 4)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERREUR : Impossible d'initialiser FreeType" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, "arial.ttf", 0, &face)) {
		std::cout << "ERREUR : Impossible de charger la police" << std::endl;
	}

    FT_Set_Pixel_Sizes(face, 0, 24);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Désactive la restriction d'alignement d'octets

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue; // erreur -> on skip

        // Génération de la texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // Configuration de la texture (Clamp to edge est crucial pour éviter les artefacts)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Stockage dans la map
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    textVAO.Bind();

    textVAO.LinkAttrib(textVBO, 0, 4, GL_FLOAT, 4 * sizeof(float), (void*)0);

    textVAO.Unbind();
}

void Polices::useTextShader(int width, int height)
{
    textShader.Activate();

    glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform1i(glGetUniformLocation(textShader.ID, "textTexture"), 0);

    glUniform3f(glGetUniformLocation(textShader.ID, "textColor"), 1.0f, 1.0f, 1.0f);

    glActiveTexture(GL_TEXTURE0);
}

void Polices::RenderText(std::string text, float x, float y, float scale)
{
    glUseProgram(textShader.ID);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO.ID);

    for (char c : text)
    {
        Character ch = Characters[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, textVBO.ID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Avancer le curseur (FreeType utilise des 1/64 pixels)
        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


Polices::~Polices()
{
    for (auto& pair : Characters)
    {
        glDeleteTextures(1, &pair.second.TextureID);
    }
    Characters.clear();
}
