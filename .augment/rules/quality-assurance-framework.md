---
type: "agent_requested"
description: "Example description"
---

# Quality Assurance Framework

## Comprehensive Validation & Testing Standards

### 🎯 QUALITY GATES & VALIDATION

#### **Pre-Development Validation**

```typescript
// Project Requirements Validation
interface ProjectRequirements {
  functional: {
    userStories: UserStory[];
    acceptanceCriteria: AcceptanceCriteria[];
    businessRules: BusinessRule[];
    dataRequirements: DataRequirement[];
  };
  nonFunctional: {
    performance: PerformanceRequirement[];
    security: SecurityRequirement[];
    accessibility: AccessibilityRequirement[];
    usability: UsabilityRequirement[];
  };
  technical: {
    architecture: ArchitectureDecision[];
    integrations: IntegrationRequirement[];
    dependencies: DependencyRequirement[];
    constraints: TechnicalConstraint[];
  };
}

// Validation Checklist
const preDevValidation = {
  requirements: {
    completeness: "All user stories have acceptance criteria",
    clarity: "Requirements are unambiguous and testable",
    feasibility: "Technical feasibility assessed and confirmed",
    priority: "Requirements prioritized using MoSCoW method",
  },
  design: {
    wireframes: "Low-fidelity wireframes approved",
    mockups: "High-fidelity designs approved",
    prototype: "Interactive prototype validated with users",
    designSystem: "Design tokens and components defined",
  },
  technical: {
    architecture: "System architecture documented and reviewed",
    database: "Data model designed and normalized",
    apis: "API contracts defined and documented",
    security: "Security requirements identified and planned",
  },
} as const;
```

#### **Development Quality Gates**

```typescript
// Code Quality Metrics
interface CodeQualityMetrics {
  coverage: {
    unit: number; // Target: >80%
    integration: number; // Target: >70%
    e2e: number; // Target: >60%
  };
  complexity: {
    cyclomatic: number; // Target: <10 per function
    cognitive: number; // Target: <15 per function
    nesting: number; // Target: <4 levels
  };
  maintainability: {
    duplication: number; // Target: <3%
    coupling: number; // Target: <0.3
    cohesion: number; // Target: >0.7
  };
  performance: {
    bundleSize: number; // Target: <150KB per route
    loadTime: number; // Target: <3s
    memoryUsage: number; // Target: <50MB
  };
}

// Automated Quality Checks
class QualityGate {
  private metrics: CodeQualityMetrics;

  constructor(metrics: CodeQualityMetrics) {
    this.metrics = metrics;
  }

  validateCoverage(): ValidationResult {
    const { unit, integration, e2e } = this.metrics.coverage;

    return {
      passed: unit >= 80 && integration >= 70 && e2e >= 60,
      details: {
        unit: { value: unit, target: 80, passed: unit >= 80 },
        integration: {
          value: integration,
          target: 70,
          passed: integration >= 70,
        },
        e2e: { value: e2e, target: 60, passed: e2e >= 60 },
      },
    };
  }

  validateComplexity(): ValidationResult {
    const { cyclomatic, cognitive, nesting } = this.metrics.complexity;

    return {
      passed: cyclomatic < 10 && cognitive < 15 && nesting < 4,
      details: {
        cyclomatic: { value: cyclomatic, target: 10, passed: cyclomatic < 10 },
        cognitive: { value: cognitive, target: 15, passed: cognitive < 15 },
        nesting: { value: nesting, target: 4, passed: nesting < 4 },
      },
    };
  }

  validatePerformance(): ValidationResult {
    const { bundleSize, loadTime, memoryUsage } = this.metrics.performance;

    return {
      passed: bundleSize < 150000 && loadTime < 3000 && memoryUsage < 52428800,
      details: {
        bundleSize: {
          value: bundleSize,
          target: 150000,
          passed: bundleSize < 150000,
        },
        loadTime: { value: loadTime, target: 3000, passed: loadTime < 3000 },
        memoryUsage: {
          value: memoryUsage,
          target: 52428800,
          passed: memoryUsage < 52428800,
        },
      },
    };
  }

  runAllChecks(): QualityReport {
    return {
      coverage: this.validateCoverage(),
      complexity: this.validateComplexity(),
      performance: this.validatePerformance(),
      timestamp: new Date().toISOString(),
      passed:
        this.validateCoverage().passed &&
        this.validateComplexity().passed &&
        this.validatePerformance().passed,
    };
  }
}
```

---

### 🧪 COMPREHENSIVE TESTING STRATEGY

#### **Testing Pyramid Implementation**

```typescript
// Unit Testing Standards
describe("Component Testing Standards", () => {
  // Test Structure: Arrange, Act, Assert
  it("should render with correct props", () => {
    // Arrange
    const props = {
      title: "Test Title",
      variant: "primary" as const,
      onClick: jest.fn(),
    };

    // Act
    render(<Button {...props} />);

    // Assert
    expect(screen.getByRole("button")).toHaveTextContent("Test Title");
    expect(screen.getByRole("button")).toHaveClass("btn-primary");
  });

  // Test Edge Cases
  it("should handle empty state gracefully", () => {
    render(<DataTable data={[]} />);
    expect(screen.getByText("No data available")).toBeInTheDocument();
  });

  // Test Error Boundaries
  it("should catch and display errors", () => {
    const ThrowError = () => {
      throw new Error("Test error");
    };

    render(
      <ErrorBoundary>
        <ThrowError />
      </ErrorBoundary>
    );

    expect(screen.getByText(/something went wrong/i)).toBeInTheDocument();
  });
});

// Integration Testing
describe("API Integration Tests", () => {
  beforeEach(() => {
    // Setup test database
    setupTestDatabase();
    // Mock external services
    mockExternalServices();
  });

  afterEach(() => {
    // Cleanup
    cleanupTestDatabase();
    restoreExternalServices();
  });

  it("should create user with valid data", async () => {
    const userData = {
      email: "test@example.com",
      password: "SecurePassword123!",
      name: "Test User",
    };

    const response = await request(app)
      .post("/api/users")
      .send(userData)
      .expect(201);

    expect(response.body).toMatchObject({
      id: expect.any(String),
      email: userData.email,
      name: userData.name,
    });

    // Verify database state
    const user = await User.findById(response.body.id);
    expect(user).toBeTruthy();
    expect(user.email).toBe(userData.email);
  });
});

// E2E Testing with Playwright
describe("User Journey Tests", () => {
  test("complete user registration flow", async ({ page }) => {
    // Navigate to registration page
    await page.goto("/register");

    // Fill registration form
    await page.fill('[data-testid="email-input"]', "test@example.com");
    await page.fill('[data-testid="password-input"]', "SecurePassword123!");
    await page.fill('[data-testid="name-input"]', "Test User");

    // Submit form
    await page.click('[data-testid="submit-button"]');

    // Verify success
    await expect(page.locator('[data-testid="success-message"]')).toBeVisible();
    await expect(page).toHaveURL("/dashboard");

    // Verify user data is displayed
    await expect(page.locator('[data-testid="user-name"]')).toHaveText(
      "Test User"
    );
  });

  test("handles form validation errors", async ({ page }) => {
    await page.goto("/register");

    // Submit empty form
    await page.click('[data-testid="submit-button"]');

    // Verify validation errors
    await expect(page.locator('[data-testid="email-error"]')).toBeVisible();
    await expect(page.locator('[data-testid="password-error"]')).toBeVisible();

    // Form should not submit
    await expect(page).toHaveURL("/register");
  });
});
```

#### **Performance Testing**

```typescript
// Performance Monitoring
class PerformanceMonitor {
  private metrics: Map<string, number[]> = new Map();

  startMeasurement(label: string): void {
    performance.mark(`${label}-start`);
  }

  endMeasurement(label: string): number {
    performance.mark(`${label}-end`);
    performance.measure(label, `${label}-start`, `${label}-end`);

    const measure = performance.getEntriesByName(label)[0];
    const duration = measure.duration;

    // Store metric
    if (!this.metrics.has(label)) {
      this.metrics.set(label, []);
    }
    this.metrics.get(label)!.push(duration);

    return duration;
  }

  getAverageTime(label: string): number {
    const times = this.metrics.get(label) || [];
    return times.reduce((sum, time) => sum + time, 0) / times.length;
  }

  getPercentile(label: string, percentile: number): number {
    const times = this.metrics.get(label) || [];
    const sorted = times.sort((a, b) => a - b);
    const index = Math.ceil((percentile / 100) * sorted.length) - 1;
    return sorted[index] || 0;
  }
}

// Load Testing Configuration
const loadTestConfig = {
  scenarios: {
    homepage: {
      executor: "constant-vus",
      vus: 50,
      duration: "5m",
      gracefulRampDown: "30s",
    },
    api_stress: {
      executor: "ramping-vus",
      startVUs: 0,
      stages: [
        { duration: "2m", target: 100 },
        { duration: "5m", target: 100 },
        { duration: "2m", target: 200 },
        { duration: "5m", target: 200 },
        { duration: "2m", target: 0 },
      ],
    },
  },
  thresholds: {
    http_req_duration: ["p(95)<500", "p(99)<1000"],
    http_req_failed: ["rate<0.01"],
    http_reqs: ["rate>100"],
  },
};
```

---

### ♿ ACCESSIBILITY VALIDATION

#### **WCAG 2.1 AA+ Compliance**

```typescript
// Accessibility Testing Suite
class AccessibilityValidator {
  private axeConfig = {
    rules: {
      "color-contrast": { enabled: true },
      "keyboard-navigation": { enabled: true },
      "focus-management": { enabled: true },
      "semantic-markup": { enabled: true },
      "aria-labels": { enabled: true },
    },
    tags: ["wcag2a", "wcag2aa", "wcag21aa"],
  };

  async validatePage(page: Page): Promise<AccessibilityReport> {
    // Run axe-core accessibility tests
    const results = await page.evaluate(() => {
      return new Promise((resolve) => {
        // @ts-ignore
        axe.run(document, this.axeConfig, (err, results) => {
          resolve(results);
        });
      });
    });

    return this.processResults(results);
  }

  async validateKeyboardNavigation(
    page: Page
  ): Promise<KeyboardNavigationReport> {
    const focusableElements = await page.$$eval(
      'button, [href], input, select, textarea, [tabindex]:not([tabindex="-1"])',
      (elements) =>
        elements.map((el) => ({
          tagName: el.tagName,
          id: el.id,
          className: el.className,
          tabIndex: el.tabIndex,
        }))
    );

    // Test tab navigation
    const navigationResults = [];
    for (let i = 0; i < focusableElements.length; i++) {
      await page.keyboard.press("Tab");
      const activeElement = await page.evaluate(() => ({
        tagName: document.activeElement?.tagName,
        id: document.activeElement?.id,
        className: document.activeElement?.className,
      }));

      navigationResults.push({
        expected: focusableElements[i],
        actual: activeElement,
        matches: this.elementsMatch(focusableElements[i], activeElement),
      });
    }

    return {
      focusableElements,
      navigationResults,
      passed: navigationResults.every((result) => result.matches),
    };
  }

  async validateScreenReader(page: Page): Promise<ScreenReaderReport> {
    // Test ARIA labels and descriptions
    const ariaElements = await page.$$eval(
      "[aria-label], [aria-labelledby], [aria-describedby]",
      (elements) =>
        elements.map((el) => ({
          tagName: el.tagName,
          ariaLabel: el.getAttribute("aria-label"),
          ariaLabelledBy: el.getAttribute("aria-labelledby"),
          ariaDescribedBy: el.getAttribute("aria-describedby"),
          textContent: el.textContent?.trim(),
        }))
    );

    // Validate semantic structure
    const headingStructure = await page.$$eval(
      "h1, h2, h3, h4, h5, h6",
      (headings) =>
        headings.map((h) => ({
          level: parseInt(h.tagName.charAt(1)),
          text: h.textContent?.trim(),
          id: h.id,
        }))
    );

    return {
      ariaElements,
      headingStructure,
      hasValidHeadingHierarchy: this.validateHeadingHierarchy(headingStructure),
      hasProperLandmarks: await this.validateLandmarks(page),
    };
  }
}

// Accessibility Component Testing
describe("Accessibility Tests", () => {
  test("should have proper ARIA labels", async () => {
    render(<Button aria-label="Close dialog">×</Button>);

    const button = screen.getByRole("button");
    expect(button).toHaveAttribute("aria-label", "Close dialog");
  });

  test("should support keyboard navigation", async () => {
    const handleClick = jest.fn();
    render(<Button onClick={handleClick}>Click me</Button>);

    const button = screen.getByRole("button");

    // Test Enter key
    fireEvent.keyDown(button, { key: "Enter", code: "Enter" });
    expect(handleClick).toHaveBeenCalledTimes(1);

    // Test Space key
    fireEvent.keyDown(button, { key: " ", code: "Space" });
    expect(handleClick).toHaveBeenCalledTimes(2);
  });

  test("should have sufficient color contrast", async () => {
    render(<Text color="primary">Important text</Text>);

    const text = screen.getByText("Important text");
    const styles = window.getComputedStyle(text);

    const contrast = calculateContrast(styles.color, styles.backgroundColor);

    expect(contrast).toBeGreaterThanOrEqual(4.5);
  });
});
```

---

### 🔒 SECURITY VALIDATION

#### **Security Testing Framework**

```typescript
// Security Validation Suite
class SecurityValidator {
  async validateInputSanitization(
    endpoint: string,
    payload: any
  ): Promise<SecurityReport> {
    const maliciousPayloads = [
      '<script>alert("XSS")</script>',
      '"; DROP TABLE users; --',
      "{{7*7}}",
      "${7*7}",
      "../../../etc/passwd",
      'javascript:alert("XSS")',
    ];

    const results = [];

    for (const maliciousPayload of maliciousPayloads) {
      const testPayload = { ...payload };

      // Test each string field with malicious input
      Object.keys(testPayload).forEach((key) => {
        if (typeof testPayload[key] === "string") {
          testPayload[key] = maliciousPayload;
        }
      });

      try {
        const response = await fetch(endpoint, {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(testPayload),
        });

        const responseText = await response.text();

        results.push({
          payload: maliciousPayload,
          status: response.status,
          containsMaliciousContent: responseText.includes(maliciousPayload),
          sanitized: !responseText.includes(maliciousPayload),
        });
      } catch (error) {
        results.push({
          payload: maliciousPayload,
          error: error.message,
          sanitized: true,
        });
      }
    }

    return {
      endpoint,
      results,
      passed: results.every((result) => result.sanitized),
    };
  }

  async validateAuthentication(
    endpoint: string
  ): Promise<AuthenticationReport> {
    // Test without authentication
    const unauthenticatedResponse = await fetch(endpoint);

    // Test with invalid token
    const invalidTokenResponse = await fetch(endpoint, {
      headers: { Authorization: "Bearer invalid-token" },
    });

    // Test with expired token
    const expiredToken = this.generateExpiredToken();
    const expiredTokenResponse = await fetch(endpoint, {
      headers: { Authorization: `Bearer ${expiredToken}` },
    });

    return {
      endpoint,
      unauthenticatedBlocked: unauthenticatedResponse.status === 401,
      invalidTokenBlocked: invalidTokenResponse.status === 401,
      expiredTokenBlocked: expiredTokenResponse.status === 401,
      passed:
        unauthenticatedResponse.status === 401 &&
        invalidTokenResponse.status === 401 &&
        expiredTokenResponse.status === 401,
    };
  }

  async validateCSRF(endpoint: string): Promise<CSRFReport> {
    // Test CSRF protection
    const response = await fetch(endpoint, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        Origin: "https://malicious-site.com",
      },
      body: JSON.stringify({ action: "delete-account" }),
    });

    return {
      endpoint,
      csrfProtected: response.status === 403 || response.status === 400,
      passed: response.status === 403 || response.status === 400,
    };
  }
}
```

---

### 📊 QUALITY REPORTING

#### **Comprehensive Quality Dashboard**

```typescript
// Quality Metrics Aggregation
interface QualityDashboard {
  overall: {
    score: number; // 0-100
    grade: "A" | "B" | "C" | "D" | "F";
    trend: "improving" | "stable" | "declining";
  };
  categories: {
    functionality: QualityCategoryScore;
    performance: QualityCategoryScore;
    accessibility: QualityCategoryScore;
    security: QualityCategoryScore;
    maintainability: QualityCategoryScore;
  };
  recommendations: QualityRecommendation[];
  history: QualityHistoryPoint[];
}

class QualityReporter {
  generateReport(metrics: AllQualityMetrics): QualityDashboard {
    const categoryScores = {
      functionality: this.calculateFunctionalityScore(metrics.testing),
      performance: this.calculatePerformanceScore(metrics.performance),
      accessibility: this.calculateAccessibilityScore(metrics.accessibility),
      security: this.calculateSecurityScore(metrics.security),
      maintainability: this.calculateMaintainabilityScore(metrics.code),
    };

    const overallScore =
      Object.values(categoryScores).reduce(
        (sum, score) => sum + score.score,
        0
      ) / 5;

    return {
      overall: {
        score: Math.round(overallScore),
        grade: this.getGrade(overallScore),
        trend: this.calculateTrend(overallScore),
      },
      categories: categoryScores,
      recommendations: this.generateRecommendations(categoryScores),
      history: this.getQualityHistory(),
    };
  }

  private getGrade(score: number): "A" | "B" | "C" | "D" | "F" {
    if (score >= 90) return "A";
    if (score >= 80) return "B";
    if (score >= 70) return "C";
    if (score >= 60) return "D";
    return "F";
  }

  private generateRecommendations(
    scores: Record<string, QualityCategoryScore>
  ): QualityRecommendation[] {
    const recommendations = [];

    Object.entries(scores).forEach(([category, score]) => {
      if (score.score < 80) {
        recommendations.push({
          category,
          priority: score.score < 60 ? "high" : "medium",
          title: `Improve ${category} score`,
          description: `Current score: ${score.score}%. ${score.issues.join(
            ", "
          )}`,
          actions: score.recommendations,
        });
      }
    });

    return recommendations.sort((a, b) =>
      a.priority === "high" ? -1 : b.priority === "high" ? 1 : 0
    );
  }
}
```
