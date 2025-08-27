#!/bin/bash
set -e

# BlackHole Simulation Docker Entrypoint Script
# Handles initialization, configuration, and graceful startup

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging function
log() {
    echo -e "${BLUE}[$(date +'%Y-%m-%d %H:%M:%S')]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
}

warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

# Signal handlers for graceful shutdown
cleanup() {
    log "Received shutdown signal, cleaning up..."
    if [ ! -z "$SIMULATION_PID" ]; then
        kill -TERM "$SIMULATION_PID" 2>/dev/null || true
        wait "$SIMULATION_PID" 2>/dev/null || true
    fi
    success "Cleanup completed"
    exit 0
}

trap cleanup SIGTERM SIGINT

# Environment setup
setup_environment() {
    log "Setting up environment..."
    
    # Create necessary directories
    mkdir -p "${BLACKHOLE_DATA_DIR:-/data}"
    mkdir -p "${BLACKHOLE_CONFIG_DIR:-/etc/blackhole}"
    
    # Set default configuration if not provided
    if [ ! -f "${BLACKHOLE_CONFIG_DIR}/config.yaml" ]; then
        log "Creating default configuration..."
        cat > "${BLACKHOLE_CONFIG_DIR}/config.yaml" << EOF
# BlackHole Simulation Configuration
simulation:
  timestep: 0.01
  max_iterations: 10000
  output_frequency: 100

physics:
  black_hole_mass: 10.0  # Solar masses
  schwarzschild_radius: auto
  enable_lensing: true
  enable_accretion_disk: true

rendering:
  width: 1200
  height: 800
  fps_limit: 60
  enable_bloom: true
  anime_style: true

logging:
  level: ${BLACKHOLE_LOG_LEVEL:-INFO}
  file: ${BLACKHOLE_DATA_DIR}/simulation.log
EOF
    fi
    
    success "Environment setup completed"
}

# Health check function
health_check() {
    if command -v /usr/local/bin/BlackHoleSimulation >/dev/null 2>&1; then
        if /usr/local/bin/BlackHoleSimulation --version >/dev/null 2>&1; then
            return 0
        fi
    fi
    return 1
}

# Pre-flight checks
preflight_checks() {
    log "Running pre-flight checks..."
    
    # Check if binary exists and is executable
    if [ ! -x "/usr/local/bin/BlackHoleSimulation" ]; then
        error "BlackHoleSimulation binary not found or not executable"
        exit 1
    fi
    
    # Check OpenGL support (if available)
    if command -v glxinfo >/dev/null 2>&1; then
        if ! glxinfo >/dev/null 2>&1; then
            warn "OpenGL not available - running in headless mode"
            export BLACKHOLE_HEADLESS=1
        fi
    fi
    
    # Check write permissions
    if [ ! -w "${BLACKHOLE_DATA_DIR:-/data}" ]; then
        error "No write permission to data directory: ${BLACKHOLE_DATA_DIR:-/data}"
        exit 1
    fi
    
    # Health check
    if ! health_check; then
        error "Health check failed"
        exit 1
    fi
    
    success "Pre-flight checks passed"
}

# Parse command line arguments
parse_args() {
    case "$1" in
        --help|-h)
            echo "BlackHole Simulation Docker Container"
            echo ""
            echo "Usage: docker run [docker-options] blackhole-sim [command] [options]"
            echo ""
            echo "Commands:"
            echo "  BlackHoleSimulation  Run the simulation (default)"
            echo "  --benchmark         Run performance benchmarks"
            echo "  --test              Run test suite"
            echo "  --version           Show version information"
            echo "  bash                Start interactive shell"
            echo ""
            echo "Environment Variables:"
            echo "  BLACKHOLE_DATA_DIR     Data directory (default: /data)"
            echo "  BLACKHOLE_CONFIG_DIR   Config directory (default: /etc/blackhole)"
            echo "  BLACKHOLE_LOG_LEVEL    Log level (default: INFO)"
            echo "  BLACKHOLE_HEADLESS     Run without display (default: auto-detect)"
            echo ""
            exit 0
            ;;
        --version)
            /usr/local/bin/BlackHoleSimulation --version
            exit 0
            ;;
        --test)
            log "Running test suite..."
            cd /workspace/build
            ctest --output-on-failure
            exit $?
            ;;
        --benchmark)
            log "Running benchmarks..."
            /usr/local/bin/BlackHoleSimulation --benchmark
            exit $?
            ;;
        bash|sh|/bin/bash|/bin/sh)
            log "Starting interactive shell..."
            exec "$@"
            ;;
    esac
}

# Main execution function
run_simulation() {
    log "Starting BlackHole Simulation..."
    
    # Build command line arguments
    ARGS=()
    
    # Add configuration file if it exists
    if [ -f "${BLACKHOLE_CONFIG_DIR}/config.yaml" ]; then
        ARGS+=("--config" "${BLACKHOLE_CONFIG_DIR}/config.yaml")
    fi
    
    # Add data directory
    ARGS+=("--data-dir" "${BLACKHOLE_DATA_DIR}")
    
    # Add headless mode if needed
    if [ "${BLACKHOLE_HEADLESS}" = "1" ]; then
        ARGS+=("--headless")
    fi
    
    # Add any additional arguments passed to the container
    ARGS+=("$@")
    
    log "Command: /usr/local/bin/BlackHoleSimulation ${ARGS[*]}"
    
    # Start simulation in background to handle signals
    /usr/local/bin/BlackHoleSimulation "${ARGS[@]}" &
    SIMULATION_PID=$!
    
    # Wait for simulation to complete
    wait $SIMULATION_PID
    SIMULATION_EXIT_CODE=$?
    
    if [ $SIMULATION_EXIT_CODE -eq 0 ]; then
        success "Simulation completed successfully"
    else
        error "Simulation exited with code: $SIMULATION_EXIT_CODE"
    fi
    
    exit $SIMULATION_EXIT_CODE
}

# Main entry point
main() {
    log "BlackHole Simulation Container Starting..."
    
    # Parse special commands first
    parse_args "$@"
    
    # Setup environment
    setup_environment
    
    # Run pre-flight checks
    preflight_checks
    
    # Run the simulation
    run_simulation "$@"
}

# Execute main function with all arguments
main "$@"
