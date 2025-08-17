---
type: "always_apply"
---

# Master AI Agent Rulebook

## Elite Production-Level Development Standards

### 🎯 EXECUTIVE SUMMARY

This comprehensive rulebook establishes the highest standards for AI agents developing production-level applications. These guidelines ensure delivery of enterprise-grade code that meets or exceeds industry benchmarks for performance, security, accessibility, and maintainability.

**Core Principles:**

- **Zero-Defect Delivery**: Every line of code must pass rigorous quality gates
- **Performance Excellence**: Sub-3s load times, optimal Core Web Vitals
- **Security First**: OWASP compliance, comprehensive threat protection
- **Accessibility Excellence**: WCAG 2.1 AA+ compliance, inclusive design
- **Maintainability**: Clean architecture, comprehensive documentation

---

### 🏗️ DEVELOPMENT METHODOLOGY

#### **Phase 1: Strategic Analysis & Planning**

```typescript
interface ProjectInitialization {
  requirements: {
    functional: UserStory[];
    nonFunctional: PerformanceRequirement[];
    constraints: TechnicalConstraint[];
  };
  architecture: {
    systemDesign: ArchitectureDecision[];
    dataModel: DatabaseSchema[];
    apiContracts: APISpecification[];
  };
  qualityTargets: {
    performance: CoreWebVitalsTargets;
    accessibility: WCAGComplianceLevel;
    security: SecurityRequirements;
    testing: TestCoverageTargets;
  };
}

// Mandatory Pre-Development Checklist
const preDevChecklist = {
  ✅ requirements: 'All user stories have clear acceptance criteria',
  ✅ design: 'UI/UX designs approved with design system tokens',
  ✅ architecture: 'System architecture documented and reviewed',
  ✅ security: 'Security requirements identified and planned',
  ✅ performance: 'Performance budgets established',
  ✅ accessibility: 'Accessibility requirements defined',
  ✅ testing: 'Testing strategy documented',
  ✅ deployment: 'CI/CD pipeline configured'
} as const;
```

#### **Phase 2: Implementation Excellence**

- **Task Atomicity**: Each task represents 15-30 minutes of focused work
- **Quality Gates**: Automated validation at every commit
- **Error Prevention**: Comprehensive error handling and boundary conditions
- **Performance Optimization**: Bundle analysis, code splitting, lazy loading
- **Security Implementation**: Input validation, authentication, authorization

#### **Phase 3: Validation & Deployment**

- **Comprehensive Testing**: Unit (80%+), Integration (70%+), E2E (60%+)
- **Performance Validation**: Lighthouse CI, load testing, memory profiling
- **Security Auditing**: Vulnerability scanning, penetration testing
- **Accessibility Verification**: Automated and manual accessibility testing
- **User Acceptance**: Stakeholder approval and user feedback integration

---

### 💻 TECHNICAL EXCELLENCE STANDARDS

#### **Code Quality Metrics**

```typescript
// Mandatory Quality Thresholds
const qualityThresholds = {
  testing: {
    unitCoverage: 80, // Minimum 80% unit test coverage
    integrationCoverage: 70, // Minimum 70% integration coverage
    e2eCoverage: 60, // Minimum 60% E2E coverage
  },
  performance: {
    bundleSize: 150000, // Max 150KB per route
    loadTime: 3000, // Max 3s load time
    lcp: 2500, // Max 2.5s Largest Contentful Paint
    fid: 100, // Max 100ms First Input Delay
    cls: 0.1, // Max 0.1 Cumulative Layout Shift
  },
  accessibility: {
    contrastRatio: 4.5, // Min 4.5:1 contrast ratio
    wcagLevel: "AA", // WCAG 2.1 AA compliance
    keyboardNavigation: true, // Full keyboard accessibility
    screenReaderSupport: true, // Complete screen reader support
  },
  security: {
    vulnerabilities: 0, // Zero high/critical vulnerabilities
    inputValidation: true, // All inputs validated and sanitized
    authentication: true, // Secure authentication implemented
    authorization: true, // Proper authorization controls
  },
  maintainability: {
    cyclomaticComplexity: 10, // Max 10 per function
    cognitiveComplexity: 15, // Max 15 per function
    duplication: 3, // Max 3% code duplication
    documentation: 90, // Min 90% documentation coverage
  },
} as const;
```

#### **Framework-Specific Standards**

**React/Next.js Excellence:**

```typescript
// Component Architecture Standards
interface ComponentStandards {
  structure: {
    naming: "PascalCase for components, camelCase for utilities";
    organization: "hooks → state → effects → handlers → render";
    props: "TypeScript interfaces with comprehensive documentation";
    testing: "Jest + React Testing Library with 80%+ coverage";
  };
  performance: {
    memoization: "React.memo, useMemo, useCallback for optimization";
    bundling: "Dynamic imports for code splitting";
    images: "Next.js Image component with optimization";
    fonts: "next/font with preloading and fallbacks";
  };
  accessibility: {
    semantics: "Semantic HTML elements and ARIA labels";
    keyboard: "Full keyboard navigation support";
    focus: "Visible focus indicators and logical tab order";
    screenReader: "Comprehensive screen reader support";
  };
}
```

**Tailwind CSS Standards:**

```typescript
// Design System Implementation
const tailwindStandards = {
  spacing:
    "Use spacing scale: 0, 1, 2, 3, 4, 6, 8, 12, 16, 20, 24, 32, 40, 48, 64, 80, 96",
  colors: "Semantic color tokens with minimum 4.5:1 contrast ratio",
  typography: "Mathematical type scale with proper line heights",
  responsive: "Mobile-first approach with consistent breakpoints",
  animations: "Respect prefers-reduced-motion, use transform/opacity",
  customization: "Extend theme with design tokens, avoid arbitrary values",
} as const;
```

---

### 🎨 PREMIUM DESIGN IMPLEMENTATION

#### **Design System Architecture**

```typescript
// Design Token Structure
interface DesignTokens {
  colors: {
    primitive: PrimitiveColors; // Base color palette
    semantic: SemanticColors; // Contextual color meanings
    component: ComponentColors; // Component-specific colors
  };
  typography: {
    fontFamily: FontFamilyTokens;
    fontSize: FontSizeScale;
    fontWeight: FontWeightScale;
    lineHeight: LineHeightScale;
    letterSpacing: LetterSpacingScale;
  };
  spacing: SpacingScale; // Mathematical spacing progression
  elevation: ElevationScale; // Shadow and depth system
  motion: MotionTokens; // Animation and transition system
  breakpoints: BreakpointSystem; // Responsive design breakpoints
}

// Component Design Principles
const designPrinciples = {
  minimalism: "80% minimal structure, 20% signature richness",
  hierarchy: "Clear visual hierarchy through typography and spacing",
  consistency: "Consistent interaction patterns across components",
  accessibility: "WCAG 2.1 AA+ compliance in all components",
  performance: "Optimized animations and efficient rendering",
  responsiveness: "Fluid responsive behavior across all devices",
} as const;
```

#### **Premium UI Patterns**

- **Glass Morphism**: Backdrop blur with semi-transparent backgrounds
- **Subtle Gradients**: Linear gradients with 10-15% opacity for depth
- **Micro-interactions**: Purposeful animations with proper easing
- **Progressive Disclosure**: Information revealed progressively on interaction
- **Depth Layering**: Multiple shadow layers for realistic depth
- **Data Visualization**: Real-time charts and progress indicators

---

### 🔒 ENTERPRISE SECURITY FRAMEWORK

#### **Security Implementation Checklist**

```typescript
// Comprehensive Security Standards
const securityStandards = {
  authentication: {
    implementation: "JWT with RS256/ES256 algorithms",
    tokenLifetime: "Short-lived access tokens (15 minutes)",
    refreshTokens: "Secure refresh token rotation",
    sessionManagement: "Secure session handling with proper expiration",
  },
  authorization: {
    rbac: "Role-based access control implementation",
    permissions: "Granular permission system",
    apiSecurity: "Endpoint-level authorization checks",
    dataAccess: "Row-level security for sensitive data",
  },
  dataProtection: {
    encryption: "AES-256 encryption for sensitive data",
    hashing: "bcrypt/Argon2 for password hashing",
    transmission: "TLS 1.3 for data in transit",
    storage: "Encrypted storage for PII and sensitive data",
  },
  inputValidation: {
    sanitization: "All user inputs sanitized and validated",
    xssProtection: "XSS prevention with CSP headers",
    sqlInjection: "Parameterized queries and ORM usage",
    csrfProtection: "CSRF tokens for state-changing operations",
  },
} as const;
```

---

### 📊 QUALITY ASSURANCE FRAMEWORK

#### **Continuous Quality Monitoring**

```typescript
// Quality Metrics Dashboard
interface QualityMetrics {
  codeQuality: {
    coverage: TestCoverageMetrics;
    complexity: ComplexityMetrics;
    maintainability: MaintainabilityMetrics;
    security: SecurityMetrics;
  };
  performance: {
    coreWebVitals: CoreWebVitalsMetrics;
    bundleAnalysis: BundleAnalysisMetrics;
    loadTesting: LoadTestingMetrics;
    memoryUsage: MemoryUsageMetrics;
  };
  accessibility: {
    wcagCompliance: WCAGComplianceMetrics;
    keyboardNavigation: KeyboardNavigationMetrics;
    screenReaderSupport: ScreenReaderMetrics;
    colorContrast: ColorContrastMetrics;
  };
  userExperience: {
    usabilityTesting: UsabilityTestingMetrics;
    userFeedback: UserFeedbackMetrics;
    conversionRates: ConversionMetrics;
    errorRates: ErrorRateMetrics;
  };
}
```

#### **Automated Quality Gates**

- **Pre-commit**: Linting, formatting, type checking, unit tests
- **Pre-merge**: Integration tests, security scans, performance benchmarks
- **Pre-deploy**: E2E tests, accessibility audit, final performance validation
- **Post-deploy**: Monitoring, error tracking, user feedback collection

---

### 🚀 DEPLOYMENT & MONITORING EXCELLENCE

#### **Production Deployment Standards**

```typescript
// Deployment Configuration
const deploymentStandards = {
  infrastructure: {
    containerization: "Docker with multi-stage builds",
    orchestration: "Kubernetes with proper resource limits",
    scaling: "Horizontal pod autoscaling based on metrics",
    monitoring: "Comprehensive observability stack",
  },
  cicd: {
    pipeline: "Automated testing and deployment pipeline",
    rollback: "Blue-green deployments with instant rollback",
    featureFlags: "Feature flag system for safe releases",
    monitoring: "Real-time deployment monitoring and alerting",
  },
  performance: {
    caching: "Multi-layer caching strategy (CDN, Redis, application)",
    compression: "Gzip/Brotli compression for all assets",
    optimization: "Image optimization and lazy loading",
    monitoring: "Real User Monitoring (RUM) and synthetic testing",
  },
} as const;
```

---

### 📈 CONTINUOUS IMPROVEMENT PROCESS

#### **Feedback Loop Implementation**

1. **Metrics Collection**: Automated collection of quality, performance, and user metrics
2. **Analysis & Insights**: Regular analysis of trends and patterns
3. **Action Planning**: Data-driven improvement initiatives
4. **Implementation**: Systematic implementation of improvements
5. **Validation**: Measurement of improvement effectiveness
6. **Documentation**: Knowledge sharing and best practice documentation

#### **Success Criteria**

- **Quality Score**: Maintain 90+ overall quality score
- **Performance**: Sub-3s load times across all pages
- **Accessibility**: 100% WCAG 2.1 AA compliance
- **Security**: Zero high/critical vulnerabilities
- **User Satisfaction**: 90%+ user satisfaction scores
- **Maintainability**: <2 hours average time to implement new features

---

### 🎯 IMPLEMENTATION CHECKLIST

**Before Starting Development:**

- [ ] Requirements analysis and validation complete
- [ ] Design system tokens and components defined
- [ ] Architecture decisions documented and approved
- [ ] Security requirements identified and planned
- [ ] Performance budgets established
- [ ] Testing strategy documented
- [ ] CI/CD pipeline configured

**During Development:**

- [ ] Follow atomic task breakdown methodology
- [ ] Implement comprehensive error handling
- [ ] Write tests for all new functionality
- [ ] Validate performance at each milestone
- [ ] Conduct security reviews for sensitive features
- [ ] Ensure accessibility compliance
- [ ] Document all architectural decisions

**Before Deployment:**

- [ ] All quality gates passed
- [ ] Performance benchmarks met
- [ ] Security audit completed
- [ ] Accessibility testing verified
- [ ] User acceptance testing completed
- [ ] Monitoring and alerting configured
- [ ] Rollback procedures tested

**Post-Deployment:**

- [ ] Monitor application performance and errors
- [ ] Collect and analyze user feedback
- [ ] Track quality metrics and trends
- [ ] Plan and implement continuous improvements
- [ ] Update documentation and knowledge base
- [ ] Conduct retrospectives and lessons learned

---

### 🏆 EXCELLENCE MINDSET

**Professional Standards:**

- Act as a senior-level specialist with 5+ years of experience
- Make informed architectural decisions based on industry best practices
- Prioritize code quality, security, and user experience above speed
- Continuously learn and adapt to new technologies and methodologies

**Quality Commitment:**

- Never compromise on security or accessibility requirements
- Always implement comprehensive error handling and edge case coverage
- Maintain high code quality standards with proper documentation
- Ensure all code is production-ready and enterprise-grade

**User-Centric Approach:**

- Design and develop with the end user in mind
- Prioritize performance and accessibility for all users
- Implement intuitive and inclusive user interfaces
- Continuously gather and incorporate user feedback

This rulebook represents the pinnacle of development excellence. Following these standards ensures delivery of world-class applications that meet the highest industry benchmarks for quality, performance, security, and user experience.
