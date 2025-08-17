#pragma once

#include "BlackHole.hpp"
#include "Particle.hpp"
#include "LightRay.hpp"
#include "AccretionDisk.hpp"
#include <vector>
#include <memory>
#include <array>
#include <string>
#include <unordered_map>

// Forward declarations for OpenGL types
struct GLFWwindow;
typedef unsigned int GLuint;
typedef int GLint;

namespace BlackHoleSim {

/**
 * @brief OpenGL-based rendering engine for black hole simulation
 * 
 * Handles all visualization aspects including spacetime grid distortion,
 * particle trajectories, light ray paths, and accretion disk rendering.
 */
class RenderingEngine {
public:
    /**
     * @brief Rendering layer flags (can be combined)
     */
    enum RenderLayer : uint32_t {
        NONE = 0,
        SPACETIME_GRID = 1 << 0,        ///< Curved spacetime grid
        PARTICLES = 1 << 1,             ///< Massive particles
        PARTICLE_TRAILS = 1 << 2,       ///< Particle trajectory trails
        LIGHT_RAYS = 1 << 3,            ///< Light ray paths
        ACCRETION_DISK = 1 << 4,        ///< Accretion disk
        BLACK_HOLE = 1 << 5,            ///< Black hole event horizon
        PHOTON_SPHERE = 1 << 6,         ///< Photon sphere
        COORDINATE_AXES = 1 << 7,       ///< Coordinate system axes
        UI_OVERLAY = 1 << 8,            ///< User interface overlay
        ALL = 0xFFFFFFFF                ///< All layers
    };

    /**
     * @brief Camera control mode
     */
    enum class CameraMode {
        FREE_LOOK,      ///< Free camera movement
        ORBIT,          ///< Orbit around black hole
        FOLLOW_PARTICLE,///< Follow specific particle
        FIXED           ///< Fixed camera position
    };

    /**
     * @brief Rendering quality settings
     */
    struct QualitySettings {
        int gridResolution = 100;           ///< Spacetime grid resolution
        int diskResolution = 200;           ///< Accretion disk resolution
        int maxTrailPoints = 1000;          ///< Maximum trail points per object
        int maxLightRays = 10000;           ///< Maximum light rays to render
        bool enableAntialiasing = true;     ///< Enable MSAA
        bool enableBloom = true;            ///< Enable bloom effect
        bool enableMotionBlur = false;      ///< Enable motion blur
        float particleSize = 2.0f;          ///< Base particle size
        float lineWidth = 1.0f;             ///< Base line width
    };

    /**
     * @brief Camera state
     */
    struct Camera {
        std::array<float, 3> position = {0.0f, 0.0f, 100.0f};  ///< Camera position
        std::array<float, 3> target = {0.0f, 0.0f, 0.0f};      ///< Look-at target
        std::array<float, 3> up = {0.0f, 1.0f, 0.0f};          ///< Up vector
        float fov = 45.0f;                                      ///< Field of view (degrees)
        float nearPlane = 0.1f;                                 ///< Near clipping plane
        float farPlane = 10000.0f;                              ///< Far clipping plane
        CameraMode mode = CameraMode::FREE_LOOK;                ///< Camera control mode
        int followParticleIndex = -1;                           ///< Index of particle to follow
    };

    /**
     * @brief Construct rendering engine
     */
    RenderingEngine();

    /**
     * @brief Destructor
     */
    ~RenderingEngine();

    /**
     * @brief Initialize OpenGL context and resources
     * @param windowWidth Window width in pixels
     * @param windowHeight Window height in pixels
     * @param windowTitle Window title
     * @return True if initialization successful
     */
    bool Initialize(int windowWidth, int windowHeight, const std::string& windowTitle);

    /**
     * @brief Shutdown and cleanup resources
     */
    void Shutdown();

    /**
     * @brief Check if window should close
     * @return True if window should close
     */
    bool ShouldClose() const;

    /**
     * @brief Begin frame rendering
     */
    void BeginFrame();

    /**
     * @brief End frame rendering and swap buffers
     */
    void EndFrame();

    /**
     * @brief Render all enabled layers
     * @param blackHole Black hole to render
     * @param particles Vector of particles to render
     * @param lightRays Vector of light rays to render
     * @param accretionDisk Accretion disk to render (optional)
     */
    void Render(const BlackHole& blackHole,
                const std::vector<std::unique_ptr<Particle>>& particles,
                const std::vector<std::unique_ptr<LightRay>>& lightRays,
                const AccretionDisk* accretionDisk = nullptr);

    /**
     * @brief Set which layers to render
     * @param layers Bitwise OR of RenderLayer flags
     */
    void SetRenderLayers(uint32_t layers) { m_renderLayers = layers; }

    /**
     * @brief Get current render layers
     * @return Bitwise OR of active RenderLayer flags
     */
    uint32_t GetRenderLayers() const { return m_renderLayers; }

    /**
     * @brief Toggle specific render layer
     * @param layer Layer to toggle
     */
    void ToggleRenderLayer(RenderLayer layer);

    /**
     * @brief Set quality settings
     * @param settings New quality settings
     */
    void SetQualitySettings(const QualitySettings& settings);

    /**
     * @brief Get current quality settings
     * @return Current quality settings
     */
    const QualitySettings& GetQualitySettings() const { return m_qualitySettings; }

    /**
     * @brief Get camera reference for modification
     * @return Reference to camera
     */
    Camera& GetCamera() { return m_camera; }

    /**
     * @brief Get camera (const)
     * @return Const reference to camera
     */
    const Camera& GetCamera() const { return m_camera; }

    /**
     * @brief Update camera based on input
     * @param deltaTime Time since last update
     */
    void UpdateCamera(double deltaTime);

    /**
     * @brief Handle window resize
     * @param width New window width
     * @param height New window height
     */
    void OnWindowResize(int width, int height);

    /**
     * @brief Get window size
     * @return [width, height] in pixels
     */
    std::array<int, 2> GetWindowSize() const;

    /**
     * @brief Get GLFW window handle
     * @return GLFW window pointer
     */
    GLFWwindow* GetWindow() const { return m_window; }

    /**
     * @brief Take screenshot
     * @param filename Output filename (PNG format)
     * @return True if successful
     */
    bool TakeScreenshot(const std::string& filename) const;

    /**
     * @brief Set background color
     * @param color Background color [r, g, b, a] (0-1 range)
     */
    void SetBackgroundColor(const std::array<float, 4>& color) { m_backgroundColor = color; }

    /**
     * @brief Get current FPS
     * @return Frames per second
     */
    double GetFPS() const { return m_fps; }

private:
    // OpenGL context
    GLFWwindow* m_window;                       ///< GLFW window
    int m_windowWidth, m_windowHeight;          ///< Window dimensions

    // Rendering state
    uint32_t m_renderLayers;                    ///< Active render layers
    QualitySettings m_qualitySettings;          ///< Quality settings
    Camera m_camera;                            ///< Camera state
    std::array<float, 4> m_backgroundColor;     ///< Background color

    // Shader programs
    std::unordered_map<std::string, GLuint> m_shaderPrograms;
    
    // Vertex buffer objects
    GLuint m_gridVAO, m_gridVBO, m_gridEBO;     ///< Spacetime grid
    GLuint m_particleVAO, m_particleVBO;        ///< Particles
    GLuint m_trailVAO, m_trailVBO;              ///< Particle trails
    GLuint m_rayVAO, m_rayVBO;                  ///< Light rays
    GLuint m_diskVAO, m_diskVBO, m_diskEBO;     ///< Accretion disk
    GLuint m_sphereVAO, m_sphereVBO, m_sphereEBO; ///< Spheres (black hole, photon sphere)

    // Framebuffers for post-processing
    GLuint m_framebuffer, m_colorTexture, m_depthTexture;
    GLuint m_bloomFramebuffer, m_bloomTexture;

    // Performance tracking
    double m_fps;                               ///< Current FPS
    double m_frameTimeAccumulator;              ///< Frame time accumulator
    int m_frameCount;                           ///< Frame counter

    // Matrix uniforms
    std::array<float, 16> m_viewMatrix;         ///< View matrix
    std::array<float, 16> m_projectionMatrix;   ///< Projection matrix
    std::array<float, 16> m_modelMatrix;        ///< Model matrix

    /**
     * @brief Initialize OpenGL context
     * @return True if successful
     */
    bool InitializeOpenGL();

    /**
     * @brief Load and compile shaders
     * @return True if successful
     */
    bool LoadShaders();

    /**
     * @brief Create vertex buffer objects
     * @return True if successful
     */
    bool CreateBuffers();

    /**
     * @brief Create framebuffers for post-processing
     * @return True if successful
     */
    bool CreateFramebuffers();

    /**
     * @brief Render spacetime grid with curvature
     * @param blackHole Black hole for curvature calculation
     */
    void RenderSpacetimeGrid(const BlackHole& blackHole);

    /**
     * @brief Render particles
     * @param particles Vector of particles to render
     */
    void RenderParticles(const std::vector<std::unique_ptr<Particle>>& particles);

    /**
     * @brief Render particle trails
     * @param particles Vector of particles with trails
     */
    void RenderParticleTrails(const std::vector<std::unique_ptr<Particle>>& particles);

    /**
     * @brief Render light rays
     * @param lightRays Vector of light rays to render
     */
    void RenderLightRays(const std::vector<std::unique_ptr<LightRay>>& lightRays);

    /**
     * @brief Render accretion disk
     * @param accretionDisk Accretion disk to render
     */
    void RenderAccretionDisk(const AccretionDisk& accretionDisk);

    /**
     * @brief Render black hole event horizon
     * @param blackHole Black hole to render
     */
    void RenderBlackHole(const BlackHole& blackHole);

    /**
     * @brief Render photon sphere
     * @param blackHole Black hole for photon sphere calculation
     */
    void RenderPhotonSphere(const BlackHole& blackHole);

    /**
     * @brief Render coordinate axes
     */
    void RenderCoordinateAxes();

    /**
     * @brief Render UI overlay
     * @param blackHole Black hole for info display
     */
    void RenderUIOverlay(const BlackHole& blackHole);

    /**
     * @brief Apply post-processing effects
     */
    void ApplyPostProcessing();

    /**
     * @brief Update view and projection matrices
     */
    void UpdateMatrices();

    /**
     * @brief Load shader from file
     * @param filename Shader filename
     * @param type Shader type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.)
     * @return Shader ID or 0 on failure
     */
    GLuint LoadShader(const std::string& filename, GLenum type);

    /**
     * @brief Create shader program from vertex and fragment shaders
     * @param vertexShader Vertex shader ID
     * @param fragmentShader Fragment shader ID
     * @return Program ID or 0 on failure
     */
    GLuint CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader);

    /**
     * @brief Generate sphere geometry
     * @param radius Sphere radius
     * @param segments Number of segments
     * @return [vertices, indices] arrays
     */
    std::pair<std::vector<float>, std::vector<unsigned int>> GenerateSphere(float radius, int segments);

    /**
     * @brief Generate grid geometry
     * @param size Grid size
     * @param resolution Grid resolution
     * @return [vertices, indices] arrays
     */
    std::pair<std::vector<float>, std::vector<unsigned int>> GenerateGrid(float size, int resolution);

    /**
     * @brief Calculate spacetime curvature at position
     * @param position Position [x, y, z]
     * @param blackHole Black hole for curvature calculation
     * @return Curvature displacement [dx, dy, dz]
     */
    std::array<float, 3> CalculateSpacetimeCurvature(const std::array<float, 3>& position,
                                                     const BlackHole& blackHole);

    /**
     * @brief Update performance metrics
     */
    void UpdatePerformanceMetrics();

    // Static callback functions for GLFW
    static void ErrorCallback(int error, const char* description);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

} // namespace BlackHoleSim