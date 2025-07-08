#include "physics/ShadowCaster.h"

//#define DEBUG_SHADOW

#include "SDL_image.h"

ShadowCaster::ShadowCaster(GameObject& associated, Vec2 offset) : Component(associated) {
    this->offset = offset;
    fixedVertices = false;
}

ShadowCaster::ShadowCaster(GameObject &associated, std::vector<Vec2> offsetVectors) : Component(associated) {
    this->offset = {0, 0};
    this->offsetVectors = offsetVectors;
    fixedVertices = true;
}

void ShadowCaster::Update(float dt) {
    if (fixedVertices) {
        vertices.clear();
        for (int i = 0; i < offsetVectors.size(); i++) {
            float x = associated.box.x + offsetVectors[i].x;
            float y = associated.box.y + offsetVectors[i].y;
            vertices.push_back({x, y});
        }
    } else {
        IsoCollider* col = (IsoCollider*) associated.GetComponent("IsoCollider");
        vertices = { col->box.TopLeft().ToCart(), col->box.TopRight().ToCart(), col->box.BottomRight().ToCart(), col->box.BottomLeft().ToCart() };
    }
}

void ShadowCaster::Render() {}

void ShadowCaster::RenderShadow(Vec2 origin) {
    std::vector<Vec2> relativeVertices;
#ifdef DEBUG_SHADOW
    int totalVertices = vertices.size() + 1;
    SDL_Point points[totalVertices];
#endif
    for (int i = 0; i < vertices.size(); i++) {
        float x = vertices[i].x + offset.x - Camera::pos.x;
        float y = vertices[i].y + offset.y - Camera::pos.y;
        relativeVertices.push_back({x, y});
#ifdef DEBUG_SHADOW
        points[i] = {(int) x, (int) y};
#endif
    }

#ifdef DEBUG_SHADOW
    points[vertices.size()] = points[0];
    SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(GAME_RENDERER, points, totalVertices);
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 255, SDL_ALPHA_OPAQUE);
#endif
    std::vector<Line> edges = GenerateEdges(relativeVertices);

    IsoCollider* col = (IsoCollider*) associated.GetComponent("IsoCollider");
    Vec2 screenPos = (col ? col->box.Center().ToCart() : associated.box.Center()) - Camera::pos;
    float relativeAngle = screenPos.Angle(origin);

    // invert = invert atan2 params (y, x) or (x, y)
    // angleMul = -y, -x or y, x
    bool invert = false;
    float angleMul = 1;
    if (relativeAngle < 3*M_PI/4 && M_PI/4 < relativeAngle) {
        // Baixo = entre 3pi/4 e pi/4
        invert = true;
    } else if (3*M_PI/4 < relativeAngle || relativeAngle < -3*M_PI/4) {
        // Esquerda = >3pi/4 ou <-3pi/4
        angleMul = -1;
    } else if (-3*M_PI/4 < relativeAngle && relativeAngle < -M_PI/4) {
        // Cima = entre -3*M_PI/4 e -pi/4
        angleMul = -1;
        invert = true;
    } else {
        // Direita = >-pi/4 ou <pi/4
    }

    // Screen vertices and edges
    relativeVertices.push_back({0, 900});
    relativeVertices.push_back({1200, 900});
    relativeVertices.push_back({0, 0});
    relativeVertices.push_back({1200, 0});
    edges.push_back({{0, 0}, {0, 900}});
    edges.push_back({{0, 900}, {1200, 900}});
    edges.push_back({{1200, 900}, {1200, 0}});
    edges.push_back({{0, 0}, {1200, 0}});

    auto gen_vertex = [](Vec2 pos) {
        return SDL_Vertex {SDL_FPoint {pos.x, pos.y}, SDL_Color{ 0, 0, 0, 255 }, SDL_FPoint{ 0, 0 } };
    };

    std::vector<SDL_Vertex> renderVertices;
    //std::cout << std::endl << "Vertices " << relativeVertices.size() << std::endl;
    for (int i = 0; i < relativeVertices.size(); i++) {
        //std::cout << "RV: " << relativeVertices[i].ToStr() << std::endl;
        std::vector<Ray::Intersection> intersections = Ray::AllIntersections(Line(origin, relativeVertices[i]), edges);
        if (i >= relativeVertices.size() - 4) { intersections.pop_back(); }
        if (intersections.size() > 1) {
            for (int j = intersections.size() - 2; j < intersections.size(); j++) {
                renderVertices.push_back(gen_vertex(intersections[j].pos));
                //std::cout << "Intersection " << j << ": " << intersections[j].pos.ToStr() << std::endl;
            }
        }
#ifdef DEBUG_SHADOW
        SDL_RenderDrawLine(GAME_RENDERER, origin.x, origin.y, relativeVertices[i].x, relativeVertices[i].y);
#endif
    }

    auto angle = [angleMul, invert](Vec2 a, Vec2 b) {
        Vec2 diff = a.Sub(b);
        if (invert) return atan2(diff.x * angleMul, diff.y * angleMul);
        return atan2(diff.y * angleMul, diff.x * angleMul);
    };

    // Radial sort
    std::sort(renderVertices.begin(), renderVertices.end(), [origin, angle](SDL_Vertex a, SDL_Vertex b) {
        Vec2 aVec = Vec2(a.position.x, a.position.y);
        Vec2 bVec = Vec2(b.position.x, b.position.y);
        float angleA = angle(aVec, origin); //aVec.Angle(origin);
        float angleB = angle(bVec, origin); //bVec.Angle(origin);
        if (abs(angleA - angleB) < 0.0001f) return aVec.Distance(origin) < bVec.Distance(origin);
        return angleA < angleB;
    });

#ifdef DEBUG_SHADOW
    std::cout << associated.name << " - Shadow Sorted Vertices List" << std::endl;
    for (int i = 0; i < renderVertices.size(); i++) {
        Vec2 vec = Vec2(renderVertices[i].position.x,  renderVertices[i].position.y);
        std::cout << vec.ToStr() << std::endl;
    }
#endif

    // Create triangle index map ({0, 1, 2}, {1, 2, 3}, {...})
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
    SDL_Texture* shadow = SDL_CreateTexture(GAME_RENDERER, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 1200, 900);
    SDL_SetTextureBlendMode(shadow, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(GAME_RENDERER, shadow);

    // Draw shadow triangles
#ifndef DEBUG_SHADOW
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderGeometry(GAME_RENDERER, nullptr, renderVertices.data(), renderVertices.size(), indices.data(), indices.size());
#endif

    // Draw hollow sprite
    SpriteRenderer* sr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
    if (sr != nullptr) {
        SDL_BlendMode bm = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
        SDL_SetTextureBlendMode(sr->sprite.texture, bm);
        sr->Render();
        SDL_SetTextureBlendMode(sr->sprite.texture, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderTarget(GAME_RENDERER, nullptr);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 1200;
    rect.h = 900;
    SDL_RenderCopy(GAME_RENDERER, shadow, &rect, &rect);
    SDL_DestroyTexture(shadow);
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

/*
 * ShadowCaster Controller
 */
ShadowCaster::Controller::Controller(GameObject &associated) : Component(associated) {}
void ShadowCaster::Controller::Update(float dt) {}

void ShadowCaster::Controller::Render() {
    if (!Character::player) return;

    IsoCollider* playerCol = (IsoCollider*) Character::player->associated.GetComponent("IsoCollider");
    Vec2 origin = playerCol->box.Center().ToCart() - Camera::pos;

    for (int i = 0; i < CURRENT_STATE.objectArray.size(); ++i) {
        GameObject* go = CURRENT_STATE.objectArray[i].get();
        ShadowCaster* sc = (ShadowCaster*) go->GetComponent("ShadowCaster");
        if (sc && go->box.Collides(Camera::PosRect())) {
            sc->RenderShadow(origin);
        }
    }
}

bool ShadowCaster::Controller::Is(std::string type) {
    return type == "ShadowCasterController";
}
