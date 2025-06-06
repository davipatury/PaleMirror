#include "physics/ShadowCaster.h"

//#define DEBUG_SHADOW

#include "SDL_image.h"

ShadowCaster::ShadowCaster(GameObject& associated, Vec2 offset) : Component(associated) {
    this->offset = offset;
    IsoRect isoRect = IsoRect(associated.box);
    vertices = { isoRect.Top(), isoRect.Right(), isoRect.Bottom(), isoRect.Left() };
}

ShadowCaster::ShadowCaster(GameObject &associated, Vec2 offset, std::vector<Vec2> vertices) : Component(associated) {
    this->vertices = vertices;
}

void ShadowCaster::Update(float dt) {

}

void ShadowCaster::Render() {
    if (Character::player == nullptr) return;

    Vec2 origin = Character::player->Pos() - Camera::pos + Vec2(0, 25);
    Vec2 polyCenter = GetVerticesCenter();
    Vec2 screenPos = polyCenter - Camera::pos;
    if (screenPos.x < 0 || screenPos.x > 1200 || screenPos.y < 0 || screenPos.y > 900) return;

    std::vector<Vec2> relativeVertices;
#ifdef DEBUG_SHADOW
    int totalVertices = vertices.size() + 1;
    SDL_Point points[totalVertices];
#endif
    for (int i = 0; i < vertices.size(); i++) {
        float x = associated.box.x + vertices[i].x + offset.x - Camera::pos.x;
        float y = associated.box.y + vertices[i].y + offset.y - Camera::pos.y;
        relativeVertices.push_back({x, y});
#ifdef DEBUG_SHADOW
        points[i] = {(int) x, (int) y};
#endif
    }

#ifdef DEBUG_SHADOW
    points[vertices.size()] = points[0];
    SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(GAME_RENDERER, points, totalVertices);
#endif

    std::vector<Line> edges = GenerateEdges(relativeVertices);
    if (screenPos.x > origin.x) {
        // Right quadrant
        if (screenPos.y > origin.y) {
            // Bottom quadrant
            relativeVertices.push_back({1200, 900});
            edges.push_back({{1200, 900}, {0, 900}});
            edges.push_back({{1200, 900}, {1200, 0}});
        } else {
            // Top quadrant
            relativeVertices.push_back({1200, 0});
            edges.push_back({{1200, 0}, {0, 0}});
            edges.push_back({{1200, 0}, {1200, 900}});
        }
    } else {
        // Left quadrant
        if (screenPos.y > origin.y) {
            // Bottom quadrant
            relativeVertices.push_back({0, 900});
            edges.push_back({{0, 900}, {0, 0}});
            edges.push_back({{0, 900}, {1200, 900}});
        } else {
            // Top quadrant
            relativeVertices.push_back({0, 0});
            edges.push_back({{0, 0}, {0, 900}});
            edges.push_back({{0, 0}, {1200, 0}});
        }
    }

    auto gen_vertex = [](Vec2 pos) {
        return SDL_Vertex {SDL_FPoint {pos.x, pos.y}, SDL_Color{ 0, 0, 0, 255 }, SDL_FPoint{ 0, 0 } };
    };

    std::vector<SDL_Vertex> renderVertices;

    //std::cout << "Vertices " << relativeVertices.size() << std::endl;
    //SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 255, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < relativeVertices.size(); i++) {
        //std::cout << "RV: " << relativeVertices[i].x << ", " << relativeVertices[i].y << std::endl;
        std::vector<Ray::Intersection> intersections = Ray::AllIntersections(Line(origin, relativeVertices[i]), edges);
        if (i == relativeVertices.size() - 1) { intersections.pop_back(); }
        if (intersections.size() > 1) {
            std::sort(intersections.begin(), intersections.end(), [](Ray::Intersection r1, Ray::Intersection r2) {
                return r1.param < r2.param;
            });
            for (int j = intersections.size() - 2; j < intersections.size(); j++) {
                renderVertices.push_back(gen_vertex(intersections[j].pos));
                //std::cout << intersections[j].pos.x << ", " << intersections[j].pos.y << std::endl;
            }
        }
        //SDL_RenderDrawLine(GAME_RENDERER, origin.x, origin.y, relativeVertices[i].x, relativeVertices[i].y);
    }

    std::sort(renderVertices.begin(), renderVertices.end(), [origin](SDL_Vertex a, SDL_Vertex b) {
        float angleA = Vec2(a.position.x, a.position.y).Angle(origin);
        float angleB = Vec2(b.position.x, b.position.y).Angle(origin);
        if (abs(angleA - angleB) < 0.00001) {
            return Vec2(a.position.x, a.position.y).Distance(origin) < Vec2(b.position.x, b.position.y).Distance(origin);
        }
        return angleA < angleB;
    });

    std::vector<int> indices;
    for (int i = 2; i < renderVertices.size(); i++) {
#ifdef DEBUG_SHADOW
        SDL_SetRenderDrawColor(GAME_RENDERER, 255, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(GAME_RENDERER, renderVertices[i-2].position.x, renderVertices[i-2].position.y, renderVertices[i-1].position.x, renderVertices[i-1].position.y);
        SDL_RenderDrawLine(GAME_RENDERER, renderVertices[i-1].position.x, renderVertices[i-1].position.y, renderVertices[i].position.x, renderVertices[i].position.y);
#endif
        indices.insert(indices.end(), { i, i - 1, i - 2 });
    }

    // Create shadow texture
    SDL_Texture* shadow = SDL_CreateTexture(GAME_RENDERER, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 1200, 900);
    SDL_SetTextureBlendMode(shadow, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(GAME_RENDERER, shadow);

    // Draw shadow
#ifndef DEBUG_SHADOW
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderGeometry(GAME_RENDERER, nullptr, renderVertices.data(), renderVertices.size(), indices.data(), indices.size());
#endif

    // Draw sprite
    /*
    SpriteRenderer* sr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
    if (sr != nullptr) {
        SDL_BlendMode blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
        SDL_SetTextureBlendMode(sr->sprite.texture, blendMode);
        sr->Render();
        SDL_SetTextureBlendMode(sr->sprite.texture, SDL_BLENDMODE_BLEND);
    }
    */

    SDL_SetRenderTarget(GAME_RENDERER, nullptr);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 1200;
    rect.h = 900;
    SDL_RenderCopy(GAME_RENDERER, shadow, &rect, &rect);
    SDL_DestroyTexture(shadow);
}

Vec2 ShadowCaster::GetVerticesCenter() {
    float totalX = 0;
    float totalY = 0;
    for (int i = 0; i < vertices.size(); i++) {
        totalX += vertices[i].x;
        totalY += vertices[i].y;
    }
    return Vec2(associated.box.x + totalX / vertices.size(), associated.box.y + totalY / vertices.size());
}

std::vector<Line> ShadowCaster::GenerateEdges(std::vector<Vec2> vertices) {
    std::vector<Line> edges;
    for (int i = 1; i < vertices.size(); i++) {
        edges.push_back(Line(vertices[i-1], vertices[i]));
    }
    edges.push_back(Line(vertices[vertices.size()-1], vertices[0]));
    return edges;
}

bool ShadowCaster::Is(std::string type) {
    return type == "ShadowCaster";
}
