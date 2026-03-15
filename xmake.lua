-- RICA/lib/Slog/xmake.lua
add_rules("mode.debug", "mode.release")

target("Slog")
    set_kind("static")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_includedirs("src/systems", {public = true}) 

    if is_mode("release") then
        add_defines("NDEBUG")
    end