---
type: "always_apply"
---

# Enhanced AI Agent Development Guidelines

## Production-Level Code Standards & Best Practices

### 🎯 CORE PROFESSIONAL STANDARDS

#### **Expertise & Specialization**

- **Domain Mastery**: Act as a senior-level specialist (5+ years experience) in the specific technology stack
- **Technology Leadership**: Stay current with latest stable releases, security patches, and industry standards
- **Architecture Decisions**: Make informed choices based on scalability, maintainability, and performance requirements
- **Code Review Mindset**: Apply the same rigor as reviewing production code for enterprise applications

#### **Quality Assurance Framework**

- **Zero-Defect Delivery**: Code must pass all automated tests, linting, and security scans before completion
- **Performance Benchmarks**: Meet or exceed industry standards (Core Web Vitals, accessibility scores)
- **Security First**: Implement OWASP Top 10 protections, input validation, and secure coding practices
- **Documentation Standards**: Maintain comprehensive inline documentation and README files

---

### 🏗️ TASK MANAGEMENT & EXECUTION

#### **Strategic Task Breakdown**

- **Atomic Tasks**: Each task represents 15-30 minutes of focused development work
- **Dependency Mapping**: Identify and sequence tasks based on technical dependencies
- **Risk Assessment**: Flag high-risk tasks requiring additional validation or testing
- **Progress Tracking**: Update task status only after successful validation and testing

#### **Validation Gates**

```typescript
interface TaskValidation {
  codeQuality: "linting" | "formatting" | "typeChecking";
  functionality: "unitTests" | "integrationTests" | "e2eTests";
  performance: "lighthouse" | "bundleSize" | "loadTime";
  accessibility: "axe" | "wcag" | "screenReader";
  security: "vulnerabilities" | "dependencies" | "codeAnalysis";
}
```

#### **Error Recovery Protocols**

- **Immediate Detection**: Monitor console errors, network failures, and runtime exceptions
- **Root Cause Analysis**: Trace errors to source, not just symptoms
- **Systematic Resolution**: Fix underlying issues, not just visible problems
- **Regression Prevention**: Add tests to prevent similar issues in the future

---

### 💻 CODE QUALITY & IMPLEMENTATION

#### **Modern Development Standards**

- **TypeScript First**: Strict mode enabled, comprehensive type definitions
- **Functional Programming**: Prefer pure functions, immutable data structures
- **Component Architecture**: Single responsibility, composable, testable components
- **Performance Optimization**: Code splitting, lazy loading, memoization where appropriate

#### **Framework-Specific Excellence**

```typescript
// React/Next.js Standards
const ComponentStandards = {
  naming: "PascalCase for components, camelCase for utilities",
  structure: "hooks -> state -> effects -> handlers -> render",
  performance: "React.memo, useMemo, useCallback for optimization",
  accessibility: "semantic HTML, ARIA labels, keyboard navigation",
};

// Tailwind CSS Standards
const StylingStandards = {
  spacing:
    "Use spacing scale: 0, 1, 2, 3, 4, 6, 8, 12, 16, 20, 24, 32, 40, 48, 64, 80, 96",
  colors: "Semantic color tokens, minimum 4.5:1 contrast ratio",
  responsive: "Mobile-first approach with consistent breakpoints",
  animations: "Respect prefers-reduced-motion, use transform/opacity",
};
```

#### **Error Handling Excellence**

- **Boundary Components**: Implement error boundaries for React applications
- **Graceful Degradation**: Provide fallbacks for failed network requests or missing data
- **User Feedback**: Clear, actionable error messages with recovery suggestions
- **Logging Strategy**: Structured logging for debugging and monitoring

---

### 🎨 DESIGN SYSTEM INTEGRATION

#### **Premium Design Standards**

- **Typography Scale**: Inter font family, mathematical scale (1.25 ratio), proper line heights
- **Color System**: Semantic tokens, WCAG AA compliance, consistent contrast ratios
- **Spacing System**: 4px base unit, mathematical progression, consistent application
- **Motion Design**: Purposeful animations, 200-320ms durations, easing functions

#### **Component Design Principles**

```typescript
interface DesignTokens {
  colors: {
    background: "#0A0A0A";
    surface: "#1A1A1A";
    elevation: "#2A2A2A";
    textPrimary: "#FFFFFF";
    textSecondary: "#A0A0A0";
    border: "#333333";
    primary: "#0066FF";
  };
  typography: {
    scale: [12, 14, 16, 18, 24, 32, 48, 64, 96];
    lineHeight: { display: 1.1; title: 1.25; body: 1.5; code: 1.45 };
    tracking: { display: "-1%"; body: "0%"; micro: "+2%" };
  };
  spacing: [0, 2, 4, 8, 12, 16, 20, 24, 32, 40, 48, 64, 80, 96];
  animation: {
    durations: { micro: 120; base: 200; medium: 280; complex: 320 };
    easings: { outQuint: [0.22, 1, 0.36, 1]; inOutCubic: [0.65, 0, 0.35, 1] };
  };
}
```

---

### 🚀 PERFORMANCE & OPTIMIZATION

#### **Core Web Vitals Targets**

- **LCP (Largest Contentful Paint)**: < 2.5 seconds
- **FID (First Input Delay)**: < 100 milliseconds
- **CLS (Cumulative Layout Shift)**: < 0.1
- **TTI (Time to Interactive)**: < 3.8 seconds

#### **Optimization Strategies**

- **Bundle Analysis**: Keep JavaScript bundles < 150KB per route
- **Image Optimization**: AVIF/WebP with JPEG fallbacks, responsive images
- **Code Splitting**: Dynamic imports for non-critical functionality
- **Caching Strategy**: Implement proper cache headers and service workers

---

### ♿ ACCESSIBILITY EXCELLENCE

#### **WCAG 2.1 AA+ Compliance**

- **Keyboard Navigation**: Full functionality without mouse interaction
- **Screen Reader Support**: Proper ARIA labels, landmarks, and descriptions
- **Color Independence**: Never rely solely on color to convey information
- **Focus Management**: Visible focus indicators, logical tab order

#### **Testing Requirements**

- **Automated Testing**: axe-core integration in CI/CD pipeline
- **Manual Testing**: Keyboard navigation, screen reader verification
- **User Testing**: Include users with disabilities in testing process
- **Documentation**: Accessibility features and usage instructions

---

### 🔒 SECURITY & BEST PRACTICES

#### **Security Implementation**

- **Input Validation**: Sanitize all user inputs, validate on client and server
- **Authentication**: Implement secure session management, proper token handling
- **Data Protection**: Encrypt sensitive data, follow GDPR/privacy regulations
- **Dependency Security**: Regular security audits, automated vulnerability scanning

#### **Code Security Standards**

```typescript
interface SecurityChecklist {
  inputValidation: "sanitization" | "validation" | "encoding";
  authentication: "JWT" | "OAuth" | "sessionManagement";
  dataProtection: "encryption" | "hashing" | "secure-storage";
  dependencies: "audit" | "update" | "vulnerability-scan";
}
```

---

### 📊 TESTING & VALIDATION

#### **Comprehensive Testing Strategy**

- **Unit Tests**: 80%+ code coverage, test business logic and edge cases
- **Integration Tests**: API endpoints, database interactions, third-party services
- **E2E Tests**: Critical user journeys, cross-browser compatibility
- **Performance Tests**: Load testing, stress testing, memory leak detection

#### **Quality Gates**

- **Pre-commit**: Linting, formatting, type checking, unit tests
- **Pre-merge**: Integration tests, security scans, performance benchmarks
- **Pre-deploy**: E2E tests, accessibility audit, final performance validation
- **Post-deploy**: Monitoring, error tracking, user feedback collection

---

### 🔄 CONTINUOUS IMPROVEMENT

#### **Monitoring & Analytics**

- **Error Tracking**: Implement comprehensive error monitoring (Sentry, LogRocket)
- **Performance Monitoring**: Real User Monitoring (RUM), synthetic testing
- **User Analytics**: Track user behavior, conversion funnels, feature usage
- **A/B Testing**: Data-driven design decisions, statistical significance

#### **Maintenance Standards**

- **Dependency Updates**: Monthly security updates, quarterly major version updates
- **Code Refactoring**: Continuous improvement, technical debt management
- **Documentation**: Keep documentation current with code changes
- **Knowledge Sharing**: Document decisions, patterns, and lessons learned
