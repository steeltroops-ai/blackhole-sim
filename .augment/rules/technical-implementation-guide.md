---
type: "agent_requested"
description: "Example description"
---

# Technical Implementation Guide

## Modern Styling & Component Architecture

### 🎨 CSS-IN-JS & MODERN STYLING

#### **Advanced Styling Architectures**

```typescript
// Styled Components with Design Tokens
import styled, { css, ThemeProvider } from "styled-components";

// Design Token Integration
const theme = {
  colors: {
    primary: {
      50: "#EFF6FF",
      500: "#3B82F6",
      900: "#1E3A8A",
    },
    semantic: {
      success: "#10B981",
      warning: "#F59E0B",
      error: "#EF4444",
    },
  },
  spacing: {
    xs: "0.25rem",
    sm: "0.5rem",
    md: "1rem",
    lg: "1.5rem",
    xl: "2rem",
  },
  typography: {
    fontFamily: {
      sans: ["Inter", "system-ui", "sans-serif"],
      mono: ["JetBrains Mono", "monospace"],
    },
    fontSize: {
      xs: "0.75rem",
      sm: "0.875rem",
      base: "1rem",
      lg: "1.125rem",
      xl: "1.25rem",
    },
  },
  animation: {
    duration: {
      fast: "150ms",
      normal: "300ms",
      slow: "500ms",
    },
    easing: {
      easeOut: "cubic-bezier(0, 0, 0.2, 1)",
      easeIn: "cubic-bezier(0.4, 0, 1, 1)",
      easeInOut: "cubic-bezier(0.4, 0, 0.2, 1)",
    },
  },
} as const;

// Advanced Component Styling
const Button = styled.button<{
  variant: "primary" | "secondary" | "ghost";
  size: "sm" | "md" | "lg";
  isLoading?: boolean;
}>`
  /* Base styles */
  display: inline-flex;
  align-items: center;
  justify-content: center;
  font-family: ${({ theme }) => theme.typography.fontFamily.sans.join(", ")};
  font-weight: 500;
  border-radius: 0.5rem;
  border: none;
  cursor: pointer;
  transition: all ${({ theme }) => theme.animation.duration.normal} ${({
      theme,
    }) => theme.animation.easing.easeOut};

  /* Size variants */
  ${({ size, theme }) => {
    switch (size) {
      case "sm":
        return css`
          padding: ${theme.spacing.xs} ${theme.spacing.sm};
          font-size: ${theme.typography.fontSize.sm};
          min-height: 2rem;
        `;
      case "lg":
        return css`
          padding: ${theme.spacing.md} ${theme.spacing.lg};
          font-size: ${theme.typography.fontSize.lg};
          min-height: 3rem;
        `;
      default:
        return css`
          padding: ${theme.spacing.sm} ${theme.spacing.md};
          font-size: ${theme.typography.fontSize.base};
          min-height: 2.5rem;
        `;
    }
  }}

  /* Variant styles */
  ${({ variant, theme }) => {
    switch (variant) {
      case "primary":
        return css`
          background: ${theme.colors.primary[500]};
          color: white;

          &:hover:not(:disabled) {
            background: ${theme.colors.primary[600]};
            transform: translateY(-1px);
            box-shadow: 0 4px 12px ${theme.colors.primary[500]}40;
          }

          &:active {
            transform: translateY(0);
          }
        `;
      case "secondary":
        return css`
          background: transparent;
          color: ${theme.colors.primary[500]};
          border: 1px solid ${theme.colors.primary[500]};

          &:hover:not(:disabled) {
            background: ${theme.colors.primary[50]};
          }
        `;
      case "ghost":
        return css`
          background: transparent;
          color: ${theme.colors.primary[500]};

          &:hover:not(:disabled) {
            background: ${theme.colors.primary[50]};
          }
        `;
    }
  }}
  
  /* Loading state */
  ${({ isLoading }) =>
    isLoading &&
    css`
      cursor: not-allowed;
      opacity: 0.7;
    `}
  
  /* Disabled state */
  &:disabled {
    cursor: not-allowed;
    opacity: 0.5;
  }

  /* Focus styles */
  &:focus-visible {
    outline: 2px solid ${({ theme }) => theme.colors.primary[500]};
    outline-offset: 2px;
  }
`;

// CSS-in-JS with Emotion
import { css, Global, ThemeProvider } from "@emotion/react";
import styled from "@emotion/styled";

const globalStyles = css`
  * {
    box-sizing: border-box;
  }

  body {
    margin: 0;
    font-family: Inter, system-ui, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
  }

  /* Custom scrollbar */
  ::-webkit-scrollbar {
    width: 8px;
  }

  ::-webkit-scrollbar-track {
    background: #f1f1f1;
  }

  ::-webkit-scrollbar-thumb {
    background: #c1c1c1;
    border-radius: 4px;
  }

  ::-webkit-scrollbar-thumb:hover {
    background: #a8a8a8;
  }
`;
```

#### **Tailwind CSS Advanced Patterns**

```typescript
// Custom Tailwind Configuration
module.exports = {
  content: ["./src/**/*.{js,ts,jsx,tsx}"],
  darkMode: "class",
  theme: {
    extend: {
      colors: {
        primary: {
          50: "#eff6ff",
          100: "#dbeafe",
          200: "#bfdbfe",
          300: "#93c5fd",
          400: "#60a5fa",
          500: "#3b82f6",
          600: "#2563eb",
          700: "#1d4ed8",
          800: "#1e40af",
          900: "#1e3a8a",
          950: "#172554",
        },
        surface: {
          background: "var(--color-background)",
          primary: "var(--color-surface-primary)",
          secondary: "var(--color-surface-secondary)",
          tertiary: "var(--color-surface-tertiary)",
        },
      },
      fontFamily: {
        sans: ["Inter", "system-ui", "sans-serif"],
        display: ["Inter Display", "system-ui", "sans-serif"],
        mono: ["JetBrains Mono", "monospace"],
      },
      fontSize: {
        "2xs": ["0.625rem", { lineHeight: "0.75rem" }],
        xs: ["0.75rem", { lineHeight: "1rem" }],
        sm: ["0.875rem", { lineHeight: "1.25rem" }],
        base: ["1rem", { lineHeight: "1.5rem" }],
        lg: ["1.125rem", { lineHeight: "1.75rem" }],
        xl: ["1.25rem", { lineHeight: "1.75rem" }],
        "2xl": ["1.5rem", { lineHeight: "2rem" }],
        "3xl": ["1.875rem", { lineHeight: "2.25rem" }],
        "4xl": ["2.25rem", { lineHeight: "2.5rem" }],
        "5xl": ["3rem", { lineHeight: "1" }],
        "6xl": ["3.75rem", { lineHeight: "1" }],
        "7xl": ["4.5rem", { lineHeight: "1" }],
        "8xl": ["6rem", { lineHeight: "1" }],
        "9xl": ["8rem", { lineHeight: "1" }],
      },
      spacing: {
        "18": "4.5rem",
        "88": "22rem",
        "128": "32rem",
      },
      animation: {
        "fade-in": "fadeIn 0.3s ease-out",
        "slide-up": "slideUp 0.4s ease-out",
        "scale-in": "scaleIn 0.3s ease-out",
        "bounce-gentle": "bounceGentle 0.6s ease-out",
        "pulse-slow": "pulse 3s cubic-bezier(0.4, 0, 0.6, 1) infinite",
      },
      keyframes: {
        fadeIn: {
          "0%": { opacity: "0" },
          "100%": { opacity: "1" },
        },
        slideUp: {
          "0%": { opacity: "0", transform: "translateY(20px)" },
          "100%": { opacity: "1", transform: "translateY(0)" },
        },
        scaleIn: {
          "0%": { opacity: "0", transform: "scale(0.9)" },
          "100%": { opacity: "1", transform: "scale(1)" },
        },
        bounceGentle: {
          "0%, 100%": { transform: "translateY(0)" },
          "50%": { transform: "translateY(-10px)" },
        },
      },
      backdropBlur: {
        xs: "2px",
        sm: "4px",
        md: "8px",
        lg: "16px",
        xl: "24px",
        "2xl": "40px",
        "3xl": "64px",
      },
    },
  },
  plugins: [
    require("@tailwindcss/forms"),
    require("@tailwindcss/typography"),
    require("@tailwindcss/aspect-ratio"),
    // Custom plugin for glassmorphism
    function ({ addUtilities }) {
      const newUtilities = {
        ".glass": {
          background: "rgba(255, 255, 255, 0.1)",
          "backdrop-filter": "blur(16px)",
          border: "1px solid rgba(255, 255, 255, 0.2)",
          "box-shadow": "0 8px 32px 0 rgba(31, 38, 135, 0.37)",
        },
        ".glass-dark": {
          background: "rgba(0, 0, 0, 0.1)",
          "backdrop-filter": "blur(16px)",
          border: "1px solid rgba(255, 255, 255, 0.1)",
          "box-shadow": "0 8px 32px 0 rgba(0, 0, 0, 0.37)",
        },
      };
      addUtilities(newUtilities);
    },
  ],
};

// Advanced Tailwind Component Patterns
const ComponentPatterns = {
  // Compound component pattern
  Card: {
    Root: "bg-white dark:bg-gray-800 rounded-lg shadow-lg overflow-hidden",
    Header: "px-6 py-4 border-b border-gray-200 dark:border-gray-700",
    Body: "px-6 py-4",
    Footer: "px-6 py-4 bg-gray-50 dark:bg-gray-700/50",
  },

  // Responsive utility patterns
  Container: "w-full max-w-7xl mx-auto px-4 sm:px-6 lg:px-8",
  Grid: "grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6",
  Flex: "flex flex-col sm:flex-row items-center justify-between gap-4",

  // Interactive state patterns
  Button: {
    Base: "inline-flex items-center justify-center font-medium rounded-lg transition-all duration-200 focus:outline-none focus:ring-2 focus:ring-offset-2",
    Primary:
      "bg-primary-500 hover:bg-primary-600 text-white focus:ring-primary-500",
    Secondary:
      "bg-gray-200 hover:bg-gray-300 text-gray-900 focus:ring-gray-500",
    Ghost: "hover:bg-gray-100 text-gray-700 focus:ring-gray-500",
  },
} as const;
```

### 🧩 COMPONENT LIBRARY ARCHITECTURE

#### **Design Token System**

```typescript
// Comprehensive Design Token Architecture
interface DesignTokens {
  colors: {
    primitive: Record<string, Record<number, string>>;
    semantic: Record<string, string>;
    component: Record<string, Record<string, string>>;
  };
  typography: {
    fontFamily: Record<string, string[]>;
    fontSize: Record<string, [string, { lineHeight: string }]>;
    fontWeight: Record<string, string>;
    letterSpacing: Record<string, string>;
  };
  spacing: Record<string, string>;
  borderRadius: Record<string, string>;
  boxShadow: Record<string, string>;
  animation: {
    duration: Record<string, string>;
    easing: Record<string, string>;
  };
}

// Token Generation System
class DesignTokenGenerator {
  private tokens: DesignTokens;

  constructor(baseTokens: Partial<DesignTokens>) {
    this.tokens = this.generateTokens(baseTokens);
  }

  private generateTokens(base: Partial<DesignTokens>): DesignTokens {
    return {
      colors: this.generateColorTokens(base.colors),
      typography: this.generateTypographyTokens(base.typography),
      spacing: this.generateSpacingTokens(base.spacing),
      borderRadius: this.generateBorderRadiusTokens(),
      boxShadow: this.generateShadowTokens(),
      animation: this.generateAnimationTokens(),
    };
  }

  private generateColorTokens(baseColors?: any): DesignTokens["colors"] {
    // Generate color scales, semantic mappings, and component tokens
    return {
      primitive: {
        gray: {
          50: "#f9fafb",
          100: "#f3f4f6",
          200: "#e5e7eb",
          300: "#d1d5db",
          400: "#9ca3af",
          500: "#6b7280",
          600: "#4b5563",
          700: "#374151",
          800: "#1f2937",
          900: "#111827",
          950: "#030712",
        },
      },
      semantic: {
        background: "var(--color-gray-50)",
        foreground: "var(--color-gray-900)",
        primary: "var(--color-blue-500)",
        secondary: "var(--color-gray-500)",
        success: "var(--color-green-500)",
        warning: "var(--color-yellow-500)",
        error: "var(--color-red-500)",
      },
      component: {
        button: {
          primary: "var(--color-primary)",
          primaryHover: "var(--color-blue-600)",
          secondary: "var(--color-gray-200)",
          secondaryHover: "var(--color-gray-300)",
        },
      },
    };
  }

  exportCSS(): string {
    // Export tokens as CSS custom properties
    return this.generateCSSVariables(this.tokens);
  }

  exportJS(): string {
    // Export tokens as JavaScript object
    return JSON.stringify(this.tokens, null, 2);
  }

  exportTailwind(): object {
    // Export tokens as Tailwind config
    return this.generateTailwindConfig(this.tokens);
  }
}

// Component Base Classes
abstract class BaseComponent {
  protected tokens: DesignTokens;
  protected variants: Record<string, any>;
  protected defaultProps: Record<string, any>;

  constructor(tokens: DesignTokens) {
    this.tokens = tokens;
    this.variants = this.defineVariants();
    this.defaultProps = this.defineDefaultProps();
  }

  abstract defineVariants(): Record<string, any>;
  abstract defineDefaultProps(): Record<string, any>;
  abstract render(props: any): string;

  protected getVariantStyles(variant: string): string {
    return this.variants[variant] || this.variants.default;
  }

  protected mergeProps(props: any): any {
    return { ...this.defaultProps, ...props };
  }
}

// Example Button Component
class ButtonComponent extends BaseComponent {
  defineVariants() {
    return {
      primary: `
        background-color: ${this.tokens.colors.semantic.primary};
        color: white;
        border: none;
        &:hover {
          background-color: ${this.tokens.colors.component.button.primaryHover};
        }
      `,
      secondary: `
        background-color: ${this.tokens.colors.component.button.secondary};
        color: ${this.tokens.colors.semantic.foreground};
        border: 1px solid ${this.tokens.colors.semantic.primary};
        &:hover {
          background-color: ${this.tokens.colors.component.button.secondaryHover};
        }
      `,
    };
  }

  defineDefaultProps() {
    return {
      variant: "primary",
      size: "md",
      disabled: false,
    };
  }

  render(props: any) {
    const mergedProps = this.mergeProps(props);
    const variantStyles = this.getVariantStyles(mergedProps.variant);

    return `
      <button 
        class="btn btn-${mergedProps.variant} btn-${mergedProps.size}"
        ${mergedProps.disabled ? "disabled" : ""}
      >
        ${mergedProps.children}
      </button>
    `;
  }
}
```

#### **Performance Optimization**

```typescript
// Component Performance Optimization
const PerformanceOptimizations = {
  // Lazy loading for large components
  lazyLoading: {
    implementation: `
      const LazyComponent = React.lazy(() => import('./HeavyComponent'));
      
      function App() {
        return (
          <Suspense fallback={<LoadingSpinner />}>
            <LazyComponent />
          </Suspense>
        );
      }
    `,
    benefits: "Reduces initial bundle size, faster page loads",
  },

  // Memoization for expensive calculations
  memoization: {
    implementation: `
      const ExpensiveComponent = React.memo(({ data, filter }) => {
        const processedData = useMemo(() => {
          return data.filter(filter).map(processItem);
        }, [data, filter]);
        
        return <DataVisualization data={processedData} />;
      });
    `,
    benefits: "Prevents unnecessary re-renders and calculations",
  },

  // Virtual scrolling for large lists
  virtualScrolling: {
    implementation: `
      import { FixedSizeList as List } from 'react-window';
      
      const VirtualizedList = ({ items }) => (
        <List
          height={600}
          itemCount={items.length}
          itemSize={50}
          itemData={items}
        >
          {({ index, style, data }) => (
            <div style={style}>
              <ListItem item={data[index]} />
            </div>
          )}
        </List>
      );
    `,
    benefits: "Handles thousands of items without performance degradation",
  },

  // Code splitting by route
  routeSplitting: {
    implementation: `
      const routes = [
        {
          path: '/',
          component: lazy(() => import('./pages/Home'))
        },
        {
          path: '/dashboard',
          component: lazy(() => import('./pages/Dashboard'))
        }
      ];
    `,
    benefits: "Loads only necessary code for each route",
  },
} as const;

// Bundle Analysis and Optimization
const BundleOptimization = {
  analysis: {
    tools: ["webpack-bundle-analyzer", "source-map-explorer", "bundlephobia"],
    metrics: [
      "Bundle size",
      "Chunk sizes",
      "Dependency tree",
      "Duplicate code",
    ],
    targets: [
      "< 150KB initial bundle",
      "< 50KB per route",
      "< 10KB per component",
    ],
  },

  techniques: {
    treeshaking: "Remove unused code from bundles",
    compression: "Gzip/Brotli compression for assets",
    minification: "Minify JavaScript, CSS, and HTML",
    caching: "Long-term caching with content hashing",
  },

  monitoring: {
    realUserMonitoring: "Track actual user performance metrics",
    syntheticTesting: "Automated performance testing",
    budgets: "Performance budgets with CI/CD integration",
    alerts: "Performance regression alerts",
  },
} as const;
```
