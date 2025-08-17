---
type: "manual"
description: "Example description"
---

# Premium Design System Implementation

## Elite-Level UI/UX Standards

### 🎨 DESIGN FOUNDATION

#### **Color System Architecture**

```typescript
// Semantic Color Tokens
export const colorTokens = {
  // Base Palette
  neutral: {
    50: "#FAFAFA",
    100: "#F5F5F5",
    200: "#E5E5E5",
    300: "#D4D4D4",
    400: "#A3A3A3",
    500: "#737373",
    600: "#525252",
    700: "#404040",
    800: "#262626",
    900: "#171717",
    950: "#0A0A0A",
  },

  // Brand Colors
  primary: {
    50: "#EFF6FF",
    100: "#DBEAFE",
    200: "#BFDBFE",
    300: "#93C5FD",
    400: "#60A5FA",
    500: "#3B82F6", // Main brand color
    600: "#2563EB",
    700: "#1D4ED8",
    800: "#1E40AF",
    900: "#1E3A8A",
    950: "#172554",
  },

  // Semantic Colors
  semantic: {
    success: "#10B981",
    warning: "#F59E0B",
    error: "#EF4444",
    info: "#3B82F6",
  },

  // Surface Colors (Dark Theme)
  surface: {
    background: "#0A0A0A",
    primary: "#1A1A1A",
    secondary: "#2A2A2A",
    tertiary: "#3A3A3A",
    overlay: "rgba(0, 0, 0, 0.8)",
    glass: "rgba(255, 255, 255, 0.05)",
  },
} as const;

// Contrast Validation
function validateContrast(foreground: string, background: string): number {
  // Implementation of WCAG contrast ratio calculation
  const luminance1 = getLuminance(foreground);
  const luminance2 = getLuminance(background);
  const brightest = Math.max(luminance1, luminance2);
  const darkest = Math.min(luminance1, luminance2);
  return (brightest + 0.05) / (darkest + 0.05);
}
```

#### **Typography System**

```typescript
// Type Scale & Hierarchy
export const typography = {
  fontFamily: {
    display: ["Inter", "system-ui", "sans-serif"],
    body: ["Inter", "system-ui", "sans-serif"],
    mono: ["JetBrains Mono", "Menlo", "Monaco", "monospace"],
  },

  fontSize: {
    xs: ["0.75rem", { lineHeight: "1rem" }], // 12px
    sm: ["0.875rem", { lineHeight: "1.25rem" }], // 14px
    base: ["1rem", { lineHeight: "1.5rem" }], // 16px
    lg: ["1.125rem", { lineHeight: "1.75rem" }], // 18px
    xl: ["1.25rem", { lineHeight: "1.75rem" }], // 20px
    "2xl": ["1.5rem", { lineHeight: "2rem" }], // 24px
    "3xl": ["1.875rem", { lineHeight: "2.25rem" }], // 30px
    "4xl": ["2.25rem", { lineHeight: "2.5rem" }], // 36px
    "5xl": ["3rem", { lineHeight: "1" }], // 48px
    "6xl": ["3.75rem", { lineHeight: "1" }], // 60px
    "7xl": ["4.5rem", { lineHeight: "1" }], // 72px
    "8xl": ["6rem", { lineHeight: "1" }], // 96px
    "9xl": ["8rem", { lineHeight: "1" }], // 128px
  },

  fontWeight: {
    thin: "100",
    extralight: "200",
    light: "300",
    normal: "400",
    medium: "500",
    semibold: "600",
    bold: "700",
    extrabold: "800",
    black: "900",
  },

  letterSpacing: {
    tighter: "-0.05em",
    tight: "-0.025em",
    normal: "0em",
    wide: "0.025em",
    wider: "0.05em",
    widest: "0.1em",
  },

  lineHeight: {
    none: "1",
    tight: "1.25",
    snug: "1.375",
    normal: "1.5",
    relaxed: "1.625",
    loose: "2",
  },
} as const;

// Typography Component System
interface TypographyProps {
  variant:
    | "h1"
    | "h2"
    | "h3"
    | "h4"
    | "h5"
    | "h6"
    | "body1"
    | "body2"
    | "caption"
    | "overline";
  color?: keyof typeof colorTokens.neutral | "primary" | "secondary";
  weight?: keyof typeof typography.fontWeight;
  align?: "left" | "center" | "right" | "justify";
}

const typographyVariants = {
  h1: "text-5xl font-bold leading-tight tracking-tight",
  h2: "text-4xl font-bold leading-tight tracking-tight",
  h3: "text-3xl font-semibold leading-snug tracking-tight",
  h4: "text-2xl font-semibold leading-snug",
  h5: "text-xl font-medium leading-snug",
  h6: "text-lg font-medium leading-normal",
  body1: "text-base leading-relaxed",
  body2: "text-sm leading-normal",
  caption: "text-xs leading-normal tracking-wide",
  overline: "text-xs font-medium uppercase tracking-widest",
} as const;
```

---

### 📐 SPACING & LAYOUT SYSTEM

#### **Mathematical Spacing Scale**

```typescript
// 4px Base Unit System
export const spacing = {
  0: "0px",
  px: "1px",
  0.5: "0.125rem", // 2px
  1: "0.25rem", // 4px
  1.5: "0.375rem", // 6px
  2: "0.5rem", // 8px
  2.5: "0.625rem", // 10px
  3: "0.75rem", // 12px
  3.5: "0.875rem", // 14px
  4: "1rem", // 16px
  5: "1.25rem", // 20px
  6: "1.5rem", // 24px
  7: "1.75rem", // 28px
  8: "2rem", // 32px
  9: "2.25rem", // 36px
  10: "2.5rem", // 40px
  11: "2.75rem", // 44px
  12: "3rem", // 48px
  14: "3.5rem", // 56px
  16: "4rem", // 64px
  20: "5rem", // 80px
  24: "6rem", // 96px
  28: "7rem", // 112px
  32: "8rem", // 128px
  36: "9rem", // 144px
  40: "10rem", // 160px
  44: "11rem", // 176px
  48: "12rem", // 192px
  52: "13rem", // 208px
  56: "14rem", // 224px
  60: "15rem", // 240px
  64: "16rem", // 256px
  72: "18rem", // 288px
  80: "20rem", // 320px
  96: "24rem", // 384px
} as const;

// Grid System
export const grid = {
  container: {
    sm: "640px",
    md: "768px",
    lg: "1024px",
    xl: "1280px",
    "2xl": "1536px",
  },

  columns: {
    1: "1",
    2: "2",
    3: "3",
    4: "4",
    5: "5",
    6: "6",
    7: "7",
    8: "8",
    9: "9",
    10: "10",
    11: "11",
    12: "12",
  },

  gap: spacing,
} as const;
```

#### **Responsive Breakpoint System**

```typescript
// Mobile-First Breakpoints
export const breakpoints = {
  xs: "0px",
  sm: "640px", // Small devices (landscape phones)
  md: "768px", // Medium devices (tablets)
  lg: "1024px", // Large devices (laptops)
  xl: "1280px", // Extra large devices (desktops)
  "2xl": "1536px", // 2X large devices (large desktops)
} as const;

// Responsive Utilities
const useResponsive = () => {
  const [breakpoint, setBreakpoint] = useState<keyof typeof breakpoints>("xs");

  useEffect(() => {
    const updateBreakpoint = () => {
      const width = window.innerWidth;
      if (width >= 1536) setBreakpoint("2xl");
      else if (width >= 1280) setBreakpoint("xl");
      else if (width >= 1024) setBreakpoint("lg");
      else if (width >= 768) setBreakpoint("md");
      else if (width >= 640) setBreakpoint("sm");
      else setBreakpoint("xs");
    };

    updateBreakpoint();
    window.addEventListener("resize", updateBreakpoint);
    return () => window.removeEventListener("resize", updateBreakpoint);
  }, []);

  return breakpoint;
};
```

---

### 🎭 ANIMATION & MOTION SYSTEM

#### **Motion Design Tokens**

```typescript
// Animation Duration Scale
export const duration = {
  instant: "0ms",
  fast: "150ms",
  normal: "300ms",
  slow: "500ms",
  slower: "750ms",
  slowest: "1000ms",
} as const;

// Easing Functions
export const easing = {
  linear: "linear",
  easeIn: "cubic-bezier(0.4, 0, 1, 1)",
  easeOut: "cubic-bezier(0, 0, 0.2, 1)",
  easeInOut: "cubic-bezier(0.4, 0, 0.2, 1)",

  // Custom Premium Easings
  outQuint: "cubic-bezier(0.22, 1, 0.36, 1)",
  inOutCubic: "cubic-bezier(0.65, 0, 0.35, 1)",
  inCubic: "cubic-bezier(0.32, 0, 0.67, 0)",

  // Bounce & Spring
  bounce: "cubic-bezier(0.68, -0.55, 0.265, 1.55)",
  spring: "cubic-bezier(0.175, 0.885, 0.32, 1.275)",
} as const;

// Motion Variants for Framer Motion
export const motionVariants = {
  fadeIn: {
    initial: { opacity: 0 },
    animate: { opacity: 1 },
    exit: { opacity: 0 },
    transition: { duration: 0.3, ease: easing.easeOut },
  },

  slideUp: {
    initial: { opacity: 0, y: 20 },
    animate: { opacity: 1, y: 0 },
    exit: { opacity: 0, y: -20 },
    transition: { duration: 0.4, ease: easing.outQuint },
  },

  scaleIn: {
    initial: { opacity: 0, scale: 0.9 },
    animate: { opacity: 1, scale: 1 },
    exit: { opacity: 0, scale: 0.9 },
    transition: { duration: 0.3, ease: easing.easeOut },
  },

  staggerContainer: {
    animate: {
      transition: {
        staggerChildren: 0.1,
        delayChildren: 0.2,
      },
    },
  },
} as const;

// Reduced Motion Support
const useReducedMotion = () => {
  const [prefersReducedMotion, setPrefersReducedMotion] = useState(false);

  useEffect(() => {
    const mediaQuery = window.matchMedia("(prefers-reduced-motion: reduce)");
    setPrefersReducedMotion(mediaQuery.matches);

    const handleChange = () => setPrefersReducedMotion(mediaQuery.matches);
    mediaQuery.addEventListener("change", handleChange);

    return () => mediaQuery.removeEventListener("change", handleChange);
  }, []);

  return prefersReducedMotion;
};
```

---

### 🧩 COMPONENT ARCHITECTURE

#### **Base Component System**

```typescript
// Component Base Props
interface BaseComponentProps {
  className?: string;
  children?: React.ReactNode;
  testId?: string;
  "aria-label"?: string;
  "aria-describedby"?: string;
}

// Button Component System
interface ButtonProps extends BaseComponentProps {
  variant: "primary" | "secondary" | "ghost" | "destructive";
  size: "sm" | "md" | "lg" | "xl";
  disabled?: boolean;
  loading?: boolean;
  leftIcon?: React.ReactNode;
  rightIcon?: React.ReactNode;
  onClick?: (event: React.MouseEvent<HTMLButtonElement>) => void;
}

const buttonVariants = {
  primary:
    "bg-primary-500 hover:bg-primary-600 text-white shadow-lg hover:shadow-xl",
  secondary:
    "bg-surface-secondary hover:bg-surface-tertiary text-neutral-100 border border-neutral-700",
  ghost: "hover:bg-surface-secondary text-neutral-300 hover:text-neutral-100",
  destructive:
    "bg-red-500 hover:bg-red-600 text-white shadow-lg hover:shadow-xl",
} as const;

const buttonSizes = {
  sm: "px-3 py-1.5 text-sm",
  md: "px-4 py-2 text-base",
  lg: "px-6 py-3 text-lg",
  xl: "px-8 py-4 text-xl",
} as const;

// Premium Button Implementation
const Button = forwardRef<HTMLButtonElement, ButtonProps>(
  (
    {
      variant = "primary",
      size = "md",
      disabled = false,
      loading = false,
      leftIcon,
      rightIcon,
      children,
      className,
      testId,
      onClick,
      ...props
    },
    ref
  ) => {
    const baseClasses =
      "inline-flex items-center justify-center font-medium rounded-lg transition-all duration-200 focus:outline-none focus:ring-2 focus:ring-primary-500 focus:ring-offset-2 focus:ring-offset-surface-background disabled:opacity-50 disabled:cursor-not-allowed";

    const variantClasses = buttonVariants[variant];
    const sizeClasses = buttonSizes[size];

    const combinedClasses = cn(
      baseClasses,
      variantClasses,
      sizeClasses,
      className
    );

    return (
      <button
        ref={ref}
        className={combinedClasses}
        disabled={disabled || loading}
        data-testid={testId}
        onClick={onClick}
        {...props}
      >
        {loading && <Spinner className="mr-2" size={size} />}
        {leftIcon && !loading && <span className="mr-2">{leftIcon}</span>}
        {children}
        {rightIcon && <span className="ml-2">{rightIcon}</span>}
      </button>
    );
  }
);
```

#### **Form Component System**

```typescript
// Input Component with Validation
interface InputProps extends BaseComponentProps {
  type?: "text" | "email" | "password" | "number" | "tel" | "url";
  placeholder?: string;
  value?: string;
  defaultValue?: string;
  disabled?: boolean;
  required?: boolean;
  error?: string;
  helperText?: string;
  leftIcon?: React.ReactNode;
  rightIcon?: React.ReactNode;
  onChange?: (event: React.ChangeEvent<HTMLInputElement>) => void;
  onBlur?: (event: React.FocusEvent<HTMLInputElement>) => void;
  onFocus?: (event: React.FocusEvent<HTMLInputElement>) => void;
}

const Input = forwardRef<HTMLInputElement, InputProps>(
  (
    {
      type = "text",
      placeholder,
      value,
      defaultValue,
      disabled = false,
      required = false,
      error,
      helperText,
      leftIcon,
      rightIcon,
      className,
      testId,
      onChange,
      onBlur,
      onFocus,
      ...props
    },
    ref
  ) => {
    const [isFocused, setIsFocused] = useState(false);

    const baseClasses =
      "w-full px-4 py-3 bg-surface-secondary border rounded-lg transition-all duration-200 placeholder:text-neutral-500 focus:outline-none focus:ring-2 focus:ring-primary-500 focus:border-transparent";

    const stateClasses = error
      ? "border-red-500 focus:ring-red-500"
      : "border-neutral-700 hover:border-neutral-600";

    const disabledClasses = disabled ? "opacity-50 cursor-not-allowed" : "";

    const combinedClasses = cn(
      baseClasses,
      stateClasses,
      disabledClasses,
      leftIcon && "pl-12",
      rightIcon && "pr-12",
      className
    );

    return (
      <div className="relative">
        {leftIcon && (
          <div className="absolute left-4 top-1/2 transform -translate-y-1/2 text-neutral-400">
            {leftIcon}
          </div>
        )}

        <input
          ref={ref}
          type={type}
          placeholder={placeholder}
          value={value}
          defaultValue={defaultValue}
          disabled={disabled}
          required={required}
          className={combinedClasses}
          data-testid={testId}
          onChange={onChange}
          onBlur={(e) => {
            setIsFocused(false);
            onBlur?.(e);
          }}
          onFocus={(e) => {
            setIsFocused(true);
            onFocus?.(e);
          }}
          {...props}
        />

        {rightIcon && (
          <div className="absolute right-4 top-1/2 transform -translate-y-1/2 text-neutral-400">
            {rightIcon}
          </div>
        )}

        {(error || helperText) && (
          <div className="mt-2 text-sm">
            {error ? (
              <span className="text-red-400 flex items-center">
                <AlertCircle className="w-4 h-4 mr-1" />
                {error}
              </span>
            ) : (
              <span className="text-neutral-500">{helperText}</span>
            )}
          </div>
        )}
      </div>
    );
  }
);
```
