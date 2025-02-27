add_rules("mode.debug", "mode.release")

set_languages("c++23")
set_toolchains("clang")

add_includedirs("include")
add_requires("gtest", "mimalloc")

target("levelDB-X")
    set_kind("static")
    add_packages("mimalloc")
    add_files("src/**.cpp")

target("levelDB-X-test")
    set_kind("binary")
    add_files("test/**.cpp")
    add_links("levelDB-X")
    add_deps("levelDB-X")
    add_packages("gtest")
    after_build(function (target)
        os.run(target:targetfile())
    end)