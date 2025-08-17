---
type: "agent_requested"
description: "Example description"
---

# Industry-Specific Design Patterns

## Specialized UI/UX Guidelines for Different Domains

### 💼 SAAS APPLICATION DESIGN

#### **SaaS Interface Patterns**

```typescript
// SaaS Application Architecture
const saasDesignPatterns = {
  navigation: {
    sidebar: {
      structure: "Logo + Primary Nav + Secondary Nav + User Profile",
      width: { collapsed: "64px", expanded: "240px" },
      behavior: "Collapsible with hover expansion",
      hierarchy: "Max 2 levels deep, 7±2 items per level",
    },
    topbar: {
      structure: "Breadcrumbs + Search + Notifications + User Menu",
      height: "56px-64px",
      sticky: true,
      responsive: "Hamburger menu on mobile",
    },
    breadcrumbs: {
      format: "Home > Section > Subsection > Current Page",
      maxItems: 4,
      truncation: "Middle truncation for long paths",
      interaction: "Clickable navigation to parent levels",
    },
  },

  dashboard: {
    layout: {
      grid: "12-column responsive grid system",
      widgets: "Draggable and resizable dashboard widgets",
      spacing: "24px gaps between widgets",
      responsive: "Stack widgets on mobile",
    },
    widgets: {
      sizes: ["1x1", "2x1", "2x2", "3x2", "4x2"],
      types: ["KPI", "Chart", "Table", "Activity Feed", "Quick Actions"],
      loading: "Skeleton loading states for all widgets",
      empty: "Meaningful empty states with actions",
    },
    kpis: {
      hierarchy: "Primary metric + secondary metrics + trend",
      visualization: "Sparklines for trend indication",
      comparison: "Period-over-period comparison",
      alerts: "Visual indicators for threshold breaches",
    },
  },

  dataVisualization: {
    charts: {
      types: ["Line", "Bar", "Pie", "Scatter", "Heatmap", "Funnel"],
      colors: "Accessible color palette with 8+ distinct colors",
      interaction: "Hover tooltips, zoom, pan, filter",
      responsive: "Adaptive chart sizing and layout",
    },
    tables: {
      features: [
        "Sorting",
        "Filtering",
        "Pagination",
        "Column resize",
        "Export",
      ],
      density: "Compact, normal, and comfortable density options",
      selection: "Row and cell selection with bulk actions",
      virtualization: "Virtual scrolling for large datasets",
    },
  },
} as const;

// SaaS User Experience Patterns
const saasUXPatterns = {
  onboarding: {
    flow: {
      steps: ["Welcome", "Setup", "Tutorial", "First Success"],
      progress: "Clear progress indication throughout",
      skippable: "Allow users to skip non-essential steps",
      contextual: "Just-in-time help and guidance",
    },
    tutorial: {
      type: "Interactive product tour with hotspots",
      timing: "Triggered on first login or feature access",
      persistence: "Remember completed steps across sessions",
      replay: "Option to replay tutorial from help menu",
    },
  },

  settings: {
    organization: {
      categories: ["Account", "Billing", "Team", "Security", "Integrations"],
      search: "Searchable settings with keyword matching",
      validation: "Real-time validation with clear error messages",
      permissions: "Role-based access to different settings",
    },
    preferences: {
      themes: "Light, dark, and auto theme options",
      notifications: "Granular notification preferences",
      language: "Multi-language support with locale detection",
      timezone: "Automatic timezone detection and manual override",
    },
  },

  collaboration: {
    sharing: {
      permissions: ["View", "Comment", "Edit", "Admin"],
      links: "Shareable links with expiration options",
      invitations: "Email invitations with role assignment",
      activity: "Real-time activity feed for shared items",
    },
    comments: {
      threading: "Nested comment threads with replies",
      mentions: "@mentions with user autocomplete",
      reactions: "Emoji reactions for quick feedback",
      notifications: "Real-time comment notifications",
    },
  },
} as const;
```

### 🛒 E-COMMERCE DESIGN PATTERNS

#### **Conversion-Focused Design**

```typescript
// E-commerce Optimization Patterns
const ecommercePatterns = {
  productDiscovery: {
    search: {
      autocomplete: "Real-time search suggestions with categories",
      filters: "Faceted search with filter counts and clear all",
      sorting: "Price, popularity, rating, newest, relevance",
      results: "Grid/list view toggle with infinite scroll",
    },
    navigation: {
      megaMenu: "Category mega menu with featured products",
      breadcrumbs: "Category > Subcategory > Product navigation",
      filters: "Sticky filter sidebar with mobile drawer",
      pagination: "Load more button or infinite scroll",
    },
    recommendations: {
      algorithms: [
        "Trending",
        "Similar",
        "Frequently bought",
        "Recently viewed",
      ],
      placement: ["Homepage", "Category", "Product", "Cart", "Checkout"],
      personalization: "User behavior and purchase history based",
      testing: "A/B testing for recommendation algorithms",
    },
  },

  productPages: {
    layout: {
      structure: "Images + Details + Actions + Reviews + Recommendations",
      images: "Zoomable gallery with 360° view option",
      variants: "Color/size selection with visual swatches",
      pricing: "Clear pricing with discounts and savings",
    },
    trust: {
      reviews: "Star ratings with review count and distribution",
      badges: "Trust badges, certifications, guarantees",
      shipping: "Free shipping thresholds and delivery estimates",
      returns: "Clear return policy and process",
    },
    actions: {
      addToCart: "Prominent add to cart with quantity selector",
      wishlist: "Save for later functionality",
      share: "Social sharing and product links",
      compare: "Product comparison functionality",
    },
  },

  checkout: {
    flow: {
      steps: ["Cart", "Shipping", "Payment", "Confirmation"],
      progress: "Clear step indicator with ability to go back",
      guestCheckout: "Option to checkout without account creation",
      autofill: "Address and payment autofill support",
    },
    optimization: {
      fields: "Minimal required fields with smart defaults",
      validation: "Real-time validation with helpful error messages",
      security: "SSL indicators and security badges",
      mobile: "Mobile-optimized checkout with large touch targets",
    },
    abandonment: {
      recovery: "Exit-intent popups with incentives",
      reminders: "Email reminders for abandoned carts",
      simplification: "One-click checkout for returning customers",
      trust: "Multiple payment options and security assurance",
    },
  },
} as const;

// Mobile Commerce Patterns
const mcommercePatterns = {
  navigation: {
    bottomNav: "Bottom navigation for primary categories",
    search: "Prominent search bar with voice search option",
    menu: "Hamburger menu with category hierarchy",
    filters: "Bottom sheet filters with apply/clear actions",
  },

  gestures: {
    swipe: "Swipe between product images and categories",
    pinch: "Pinch to zoom on product images",
    pull: "Pull to refresh product listings",
    scroll: "Infinite scroll for product discovery",
  },

  performance: {
    loading: "Progressive image loading with placeholders",
    caching: "Aggressive caching for product data",
    offline: "Offline browsing with sync when online",
    speed: "Sub-3s page load times on 3G networks",
  },
} as const;
```

### 🎨 PORTFOLIO & CREATIVE SHOWCASE

#### **Creative Portfolio Patterns**

```typescript
// Portfolio Design Architecture
const portfolioPatterns = {
  layout: {
    hero: {
      structure: "Name + Tagline + Featured Work + CTA",
      animation: "Subtle parallax or scroll-triggered animations",
      media: "High-quality hero image or video background",
      typography: "Large, bold typography with personality",
    },
    grid: {
      masonry: "Pinterest-style masonry layout for varied content",
      uniform: "Consistent grid for professional presentation",
      featured: "Featured work section with larger thumbnails",
      categories: "Filterable categories with smooth transitions",
    },
    navigation: {
      minimal: "Clean, minimal navigation that doesn't compete",
      sticky: "Sticky navigation that appears on scroll",
      contextual: "Context-aware navigation based on section",
      mobile: "Mobile-first navigation with smooth animations",
    },
  },

  projectPages: {
    storytelling: {
      structure: "Problem + Process + Solution + Results",
      media: "High-quality images, videos, and interactive demos",
      typography: "Readable typography with generous whitespace",
      flow: "Logical flow that guides the viewer through the story",
    },
    details: {
      metadata: "Project type, duration, tools, team members",
      challenges: "Problems solved and constraints overcome",
      process: "Design process and methodology used",
      outcomes: "Results, metrics, and impact achieved",
    },
    navigation: {
      previous: "Previous/next project navigation",
      related: "Related projects and case studies",
      back: "Clear path back to portfolio overview",
      share: "Social sharing for individual projects",
    },
  },

  performance: {
    images: {
      optimization: "WebP/AVIF with JPEG fallbacks",
      lazy: "Lazy loading with intersection observer",
      responsive: "Responsive images with srcset",
      compression: "Optimal compression without quality loss",
    },
    animations: {
      performance: "GPU-accelerated animations using transform/opacity",
      reduced: "Respect prefers-reduced-motion setting",
      progressive: "Progressive enhancement for animations",
      budget: "Animation performance budget monitoring",
    },
  },
} as const;

// Creative Industry Adaptations
const creativeAdaptations = {
  photography: {
    gallery: "Lightbox gallery with keyboard navigation",
    metadata: "EXIF data display for technical details",
    prints: "Print ordering and sizing options",
    watermarks: "Subtle watermarking for protection",
  },

  design: {
    process: "Design process documentation with sketches",
    tools: "Tools and software used in creation",
    iterations: "Before/after comparisons and iterations",
    rationale: "Design decisions and reasoning",
  },

  development: {
    code: "Syntax-highlighted code snippets",
    demos: "Live demos and interactive prototypes",
    github: "GitHub integration for code repositories",
    stack: "Technology stack and architecture details",
  },
} as const;
```

### 📊 DASHBOARD & DATA VISUALIZATION

#### **Dashboard Design Excellence**

```typescript
// Dashboard Architecture Patterns
const dashboardPatterns = {
  layout: {
    hierarchy: {
      overview: "High-level KPIs and summary metrics",
      details: "Detailed views with drill-down capability",
      actions: "Actionable insights and recommendations",
      alerts: "Critical alerts and notifications",
    },
    grid: {
      responsive: "12-column grid that adapts to screen size",
      widgets: "Modular widgets that can be rearranged",
      spacing: "Consistent 24px spacing between elements",
      density: "Adjustable density for information display",
    },
    navigation: {
      tabs: "Tab-based navigation for different views",
      filters: "Global filters that affect all widgets",
      timeRange: "Time range selector with presets",
      export: "Export functionality for reports and data",
    },
  },

  dataVisualization: {
    charts: {
      selection: "Choose appropriate chart type for data",
      colors: "Accessible color palette with semantic meaning",
      interaction: "Hover states, tooltips, and drill-down",
      responsive: "Charts that adapt to container size",
    },
    tables: {
      sorting: "Multi-column sorting with visual indicators",
      filtering: "Column-level filtering with search",
      pagination: "Efficient pagination for large datasets",
      export: "CSV/Excel export functionality",
    },
    maps: {
      choropleth: "Geographic data visualization with color coding",
      markers: "Point data with clustering for performance",
      interaction: "Zoom, pan, and click interactions",
      layers: "Multiple data layers with toggle controls",
    },
  },

  realTime: {
    updates: {
      websockets: "Real-time data updates via WebSocket",
      polling: "Intelligent polling with exponential backoff",
      indicators: "Visual indicators for live data",
      performance: "Optimized updates to prevent UI blocking",
    },
    alerts: {
      thresholds: "Configurable alert thresholds",
      notifications: "Toast notifications for critical alerts",
      history: "Alert history and acknowledgment",
      escalation: "Alert escalation and routing",
    },
  },
} as const;

// Analytics Dashboard Patterns
const analyticsPatterns = {
  metrics: {
    kpis: {
      primary: "1-3 primary metrics prominently displayed",
      secondary: "Supporting metrics with context",
      trends: "Trend indicators with period comparison",
      targets: "Goal tracking with progress indicators",
    },
    segmentation: {
      dimensions: "Multiple dimension analysis (time, geography, demographics)",
      cohorts: "Cohort analysis for user behavior",
      funnels: "Conversion funnel visualization",
      attribution: "Attribution modeling for marketing",
    },
  },

  exploration: {
    drilling: {
      down: "Drill down from summary to detail views",
      across: "Drill across related dimensions",
      through: "Drill through to underlying data",
      up: "Roll up to higher-level summaries",
    },
    filtering: {
      global: "Global filters that affect all visualizations",
      local: "Widget-specific filters",
      saved: "Saved filter combinations",
      sharing: "Shareable filtered views",
    },
  },
} as const;
```

### 📱 MOBILE-FIRST DESIGN STRATEGIES

#### **Progressive Enhancement Patterns**

```typescript
// Mobile-First Implementation
const mobileFirstPatterns = {
  breakpoints: {
    strategy: "Design for smallest screen first, enhance upward",
    points: {
      mobile: "320px-767px",
      tablet: "768px-1023px",
      desktop: "1024px+",
      large: "1440px+",
    },
    testing: "Test on actual devices, not just browser resize",
  },

  touch: {
    targets: {
      minimum: "44x44px minimum touch target size",
      spacing: "8px minimum spacing between targets",
      feedback: "Visual and haptic feedback for interactions",
      gestures: "Support for common mobile gestures",
    },
    navigation: {
      thumb: "Design for thumb navigation zones",
      reachability: "Important actions within thumb reach",
      bottom: "Bottom navigation for primary actions",
      floating: "Floating action buttons for key tasks",
    },
  },

  performance: {
    loading: {
      critical: "Critical path CSS inlined",
      progressive: "Progressive image loading",
      lazy: "Lazy loading for below-fold content",
      preload: "Preload critical resources",
    },
    network: {
      offline: "Offline functionality with service workers",
      slow: "Graceful degradation on slow networks",
      adaptive: "Adaptive loading based on connection speed",
      caching: "Aggressive caching for mobile performance",
    },
  },
} as const;

// Responsive Design Patterns
const responsivePatterns = {
  layout: {
    fluid: "Fluid grids that adapt to any screen size",
    flexible: "Flexible images and media",
    breakpoints: "Major breakpoints for layout changes",
    container: "Container queries for component-based responsive design",
  },

  typography: {
    scale: "Responsive typography scale using clamp()",
    readability: "Optimal line length and spacing for each device",
    hierarchy: "Consistent hierarchy across all screen sizes",
    performance: "Font loading optimization for mobile",
  },

  images: {
    responsive: "Responsive images with srcset and sizes",
    art: "Art direction for different aspect ratios",
    formats: "Modern image formats with fallbacks",
    lazy: "Lazy loading with intersection observer",
  },
} as const;
```
