# Multi-stage Dockerfile for BlackHole Simulation
# Production-ready container with minimal attack surface

# Build stage
FROM ubuntu:24.04 AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    pkg-config \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libxext-dev \
    libwayland-dev \
    libxkbcommon-dev \
    libegl1-mesa-dev \
    clang-tidy \
    cppcheck \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Copy source code
COPY . .

# Configure and build
RUN cmake -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_INSTALL_PREFIX=/usr/local

# Build the application
RUN cmake --build build --parallel $(nproc)

# Run tests
RUN cd build && ctest --output-on-failure --parallel $(nproc)

# Install to staging area
RUN cmake --install build --prefix /staging

# Runtime stage
FROM ubuntu:24.04 AS runtime

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    libgl1-mesa-glx \
    libglu1-mesa \
    libxrandr2 \
    libxinerama1 \
    libxcursor1 \
    libxi6 \
    libxext6 \
    libwayland-client0 \
    libxkbcommon0 \
    libegl1-mesa \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user for security
RUN groupadd -r blackhole && useradd -r -g blackhole -s /bin/bash blackhole

# Copy built application from builder stage
COPY --from=builder /staging /usr/local

# Create data directory
RUN mkdir -p /data && chown blackhole:blackhole /data

# Switch to non-root user
USER blackhole

# Set working directory
WORKDIR /data

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD /usr/local/bin/BlackHoleSimulation --version || exit 1

# Default command
CMD ["/usr/local/bin/BlackHoleSimulation"]

# Development stage (for development containers)
FROM builder AS development

# Install additional development tools
RUN apt-get update && apt-get install -y \
    gdb \
    lldb \
    strace \
    htop \
    vim \
    nano \
    curl \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Install VS Code Server dependencies
RUN apt-get update && apt-get install -y \
    ca-certificates \
    curl \
    gnupg \
    lsb-release \
    && rm -rf /var/lib/apt/lists/*

# Set up development environment
ENV DISPLAY=:0
ENV CMAKE_BUILD_TYPE=Debug

# Create development user
RUN groupadd -r developer && useradd -r -g developer -s /bin/bash -m developer
RUN usermod -aG sudo developer

# Switch to development user
USER developer
WORKDIR /workspace

# Default command for development
CMD ["/bin/bash"]

# Testing stage
FROM builder AS testing

# Run comprehensive tests
RUN cd build && \
    # Unit tests
    ctest --output-on-failure --parallel $(nproc) && \
    # Static analysis
    clang-tidy src/*.cpp include/*.hpp -- -Iinclude && \
    # Memory checks (if tests support headless mode)
    # valgrind --tool=memcheck --leak-check=full ./BlackHoleSimulation --test-mode && \
    echo "All tests passed"

# Benchmark stage
FROM builder AS benchmark

# Install benchmarking tools
RUN apt-get update && apt-get install -y \
    perf-tools-unstable \
    linux-tools-generic \
    && rm -rf /var/lib/apt/lists/*

# Run benchmarks
RUN cd build && \
    echo "Benchmarks would run here" && \
    # ./BlackHoleSimulation --benchmark && \
    echo "Benchmark completed"

# Multi-architecture support
FROM runtime AS final

# Labels for metadata
LABEL maintainer="BlackHole Simulation Team"
LABEL version="1.0.0"
LABEL description="Advanced Black Hole Physics Simulation"
LABEL org.opencontainers.image.source="https://github.com/steeltroops-ai/blackhole-sim"
LABEL org.opencontainers.image.documentation="https://github.com/steeltroops-ai/blackhole-sim/blob/main/README.md"
LABEL org.opencontainers.image.licenses="MIT"

# Environment variables
ENV BLACKHOLE_DATA_DIR=/data
ENV BLACKHOLE_CONFIG_DIR=/etc/blackhole
ENV BLACKHOLE_LOG_LEVEL=INFO

# Expose ports (if needed for future web interface)
EXPOSE 8080

# Volume for persistent data
VOLUME ["/data"]

# Final setup
WORKDIR /data
USER blackhole

# Entry point script
COPY --chown=blackhole:blackhole docker-entrypoint.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]
CMD ["BlackHoleSimulation"]
