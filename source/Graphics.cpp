#include <Engine.h>

#include <glad/glad.h>

Graphics::Graphics(std::shared_ptr<Image> image)
{
    m_image = image;
}

void Graphics::Clear(Pixel color)
{
    m_image->Clear(color);
}

void Graphics::SetPixel(Pixel color, unsigned x, unsigned y)
{
    m_image->SetPixel(x, y, color);
}

void Graphics::DrawLine(Pixel color, float x0_in, float y0_in, float x1_in, float y1_in)
{
    Transform transform = GetTransform();
    Vector2 p0 = transform.Apply(Vector2(x0_in, y0_in));
    Vector2 p1 = transform.Apply(Vector2(x1_in, y1_in));

    int x0 = p0.x;
    int y0 = p0.y;
    int x1 = p1.x;
    int y1 = p1.y;

    // Bresenham's line algorithm
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (true)
    {
        SetPixel(color, x0, y0);

        if (x0 == x1 && y0 == y1)
        {
            break;
        }

        e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void Graphics::DrawRect(Pixel color, unsigned x_in, unsigned y_in, unsigned width_in, unsigned height_in)
{
    Transform transform = GetTransform();
    Vector2 position = transform.Apply(Vector2(x_in, y_in));
    float x = position.x;
    float y = position.y;
    float width = width_in * transform.GetScale();
    float height = height_in * transform.GetScale();

    for (unsigned i = 0; i < width; i++)
    {
        SetPixel(color, x + i, y);
        SetPixel(color, x + i, y + height - 1);
    }

    for (unsigned i = 0; i < height; i++)
    {
        SetPixel(color, x, y + i);
        SetPixel(color, x + width - 1, y + i);
    }
}

void Graphics::DrawCircle(Pixel color, float x_in, float y_in, float radius_in)
{
    Transform transform = GetTransform();
    Vector2 position = transform.Apply(Vector2(x_in, y_in));
    float x = position.x;
    float y = position.y;
    float radius = radius_in * transform.GetScale();

    // Draw Octants
    int x0 = radius;
    int y0 = 0;
    int err = 0;

    while (x0 >= y0)
    {
        SetPixel(color, x + x0, y + y0);
        SetPixel(color, x + y0, y + x0);
        SetPixel(color, x - y0, y + x0);
        SetPixel(color, x - x0, y + y0);
        SetPixel(color, x - x0, y - y0);
        SetPixel(color, x - y0, y - x0);
        SetPixel(color, x + y0, y - x0);
        SetPixel(color, x + x0, y - y0);

        y0++;
        err += 1 + 2 * y0;

        if (2 * (err - x0) + 1 > 0)
        {
            x0--;
            err += 1 - 2 * x0;
        }
    }
}

void Graphics::FillRect(Pixel color, unsigned x_in, unsigned y_in, unsigned w_in, unsigned h_in)
{
    Transform transform = GetTransform();
    Vector2 position = transform.Apply(Vector2(x_in, y_in));
    float x = position.x;
    float y = position.y;
    float w = w_in * transform.GetScale();
    float h = h_in * transform.GetScale();

    for (unsigned i = 0; i < w; i++)
    {
        for (unsigned j = 0; j < h; j++)
        {
            SetPixel(color, x + i, y + j);
        }
    }
}

void Graphics::FillCircle(Pixel color, unsigned x_in, unsigned y_in, unsigned radius_in)
{
    Transform transform = GetTransform();
    Vector2 position = transform.Apply(Vector2(x_in, y_in));
    float x = position.x;
    float y = position.y;
    float radius = radius_in * transform.GetScale();

    // Draw Octants
    int x0 = radius;
    int y0 = 0;
    int err = 0;

    while (x0 >= y0)
    {
        DrawLine(color, x - x0, y + y0, x + x0, y + y0);
        DrawLine(color, x - y0, y + x0, x + y0, y + x0);
        DrawLine(color, x - x0, y - y0, x + x0, y - y0);
        DrawLine(color, x - y0, y - x0, x + y0, y - x0);

        y0++;
        err += 1 + 2 * y0;

        if (2 * (err - x0) + 1 > 0)
        {
            x0--;
            err += 1 - 2 * x0;
        }
    }
}

void Graphics::DrawImage(const Image &image, unsigned x, unsigned y, unsigned w, unsigned h)
{
    for (unsigned i = 0; i < w; i++)
    {
        for (unsigned j = 0; j < h; j++)
        {
            float u = (float)i / (float)w;
            float v = (float)j / (float)h;
            Pixel color = image.SamplePixel(u, v);
            SetPixel(color, x + i, y + j);
        }
    }
}

unsigned Graphics::GetWidth() const
{
    return m_image->GetWidth();
}

unsigned Graphics::GetHeight() const
{
    return m_image->GetHeight();
}

Vector2 Graphics::GetCenter() const
{
    return Vector2(GetWidth() / 2.0f, GetHeight() / 2.0f);
}

Vector2 Graphics::GetDimensions() const
{
    return Vector2(GetWidth(), GetHeight());
}

void Graphics::PushTransform(const Transform &transform)
{
    m_transform_stack.push(transform);
}

Transform Graphics::PopTransform()
{
    if (m_transform_stack.empty())
        return Transform::Identity();
    Transform transform = m_transform_stack.top();
    m_transform_stack.pop();
    return transform;
}

Transform Graphics::GetTransform() const
{
    if (m_transform_stack.empty())
        return Transform::Identity();
    return m_transform_stack.top();
}