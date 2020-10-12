#include <gtest/gtest.h>

#include "../src/DMA_Plantuml.hpp"

PUML_PACKAGE_BEGIN(test_main)
    PUML_INTERFACE_BEGIN(IInterface)
        PUML_VIRTUAL_METHOD(+, ~IInterface())
        PUML_VIRTUAL_METHOD(+, void virtFunc() )
        PUML_PURE_VIRTUAL_METHOD(+, void pureVirtFunc(const int& val1, const int& val2) )
    PUML_INTERFACE_END()

    PUML_CLASS_BEGIN(CImpl_Test)
        PUML_INHERITANCE(IInterface, implements)
        PUML_INHERITANCE(IInterfaceHelper, implements)
        PUML_INHERITANCE(UndeclaredClass, extends)
        PUML_METHOD(+, CImpl_Test())
        PUML_OVERRIDE_METHOD(+, void virtFunc() )
        PUML_OVERRIDE_METHOD(+, void pureVirtFunc(const int& val1, const int& val2) )
        PUML_STATIC_METHOD(+, void myStaticMethod() )
        PUML_COMPOSITION_DEPENDENCY(CompositionDependency, 1, 1, contains)
        PUML_AGGREGATION_DEPENDENCY(AggregationDependency, 1, 1, uses)
    PUML_CLASS_END()
PUML_PACKAGE_END()

PUML_PACKAGE_BEGIN(test_helper)

    PUML_INTERFACE_BEGIN(IInterfaceHelper)
    PUML_INTERFACE_END()

    PUML_INTERFACE_BEGIN(IDependency)
    PUML_INTERFACE_END()

    PUML_CLASS_BEGIN(CompositionDependency)
        PUML_INHERITANCE(IDependency, implements)
        PUML_AGGREGATION_DEPENDENCY(ExternalDependency, 1, 1, uses)
    PUML_CLASS_END()

    PUML_CLASS_BEGIN(AggregationDependency)
        PUML_INHERITANCE(IDependency, implements)
        PUML_AGGREGATION_DEPENDENCY(ExternalDependency, 1, 1, uses)
    PUML_CLASS_END()

    PUML_CLASS_BEGIN(CImpl_Helper)
        PUML_INHERITANCE(IInterface, implements)
        PUML_METHOD(+, CImpl_Helper())
        PUML_OVERRIDE_METHOD(+, void virtFunc() )
        PUML_OVERRIDE_METHOD(+, void pureVirtFunc(const int& val1, const int& val2) )
        PUML_STATIC_METHOD(+, void myStaticMethod() )
    PUML_CLASS_END()

    PUML_CLASS_BEGIN(CImpl_HelperExtended)
        PUML_INHERITANCE(CImpl_Helper, extends)
        PUML_METHOD(+, CImpl_HelperExtended())
        PUML_OVERRIDE_METHOD(+, void virtFunc() )
    PUML_CLASS_END()

PUML_PACKAGE_END()

PUML_PACKAGE_BEGIN(test_external)
    PUML_CLASS_BEGIN(ExternalDependency)
    PUML_CLASS_END()
PUML_PACKAGE_END()


class Test_CClassUnderTest : public ::testing::Test
{
protected:
    void SetUp()
    {
        if(false == DMA::PlantUML::Creator::getInstance().isInitislized())
        {
            DMA::PlantUML::Creator::getInstance().initialize();
        }
    }

    void TearDown()
    {}
};

TEST_F(Test_CClassUnderTest, test_full_diagram_non_emptiness)
{
    auto diagramResult = DMA::PlantUML::Creator::getInstance().getClassDiagram();
    ASSERT_EQ(diagramResult.bIsSuccessful, true);
    ASSERT_EQ(diagramResult.diagramContent.empty(), false);
}

TEST_F(Test_CClassUnderTest, test_package_diagram_non_emptiness)
{
    auto diagramResult = DMA::PlantUML::Creator::getInstance().getPackageClassDiagram("test_main");
    ASSERT_EQ(diagramResult.bIsSuccessful, true);
    ASSERT_EQ(diagramResult.diagramContent.empty(), false);
}

TEST_F(Test_CClassUnderTest, test_package_diagram_excluded_dependencies_non_emptiness)
{
    auto diagramResult = DMA::PlantUML::Creator::getInstance().getPackageClassDiagram("test_main", true);
    ASSERT_EQ(diagramResult.bIsSuccessful, true);
    ASSERT_EQ(diagramResult.diagramContent.empty(), false);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
