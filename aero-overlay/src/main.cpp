#include <render/Device2D.hpp>
#include <render/Device3D9.hpp>
using namespace render;

std::unique_ptr<Overlay> create_overlay( const EDeviceType type, const std::string& window_title )
{
    auto overlay = Overlay::New( type );

	
    if( !overlay ) {
        return nullptr;
    }
    if( !overlay->create( window_title ) ) {
        return nullptr;
    }

    auto surface = overlay->get_surface();
    if( !surface ) {
        return nullptr;
    }

    const auto is_d3d9 = type == EDeviceType::Direct3D9;

    if( !surface->add_font(
        "default",
        "Segoe UI",
        is_d3d9 ? 18 : 12,
        is_d3d9 ? FW_HEAVY : DWRITE_FONT_WEIGHT_NORMAL,
        /// or DEFAULT_QUALITY instead of PROOF_QUALITY for anti aliasing
        is_d3d9 ? PROOF_QUALITY : DWRITE_FONT_STRETCH_NORMAL
    ) ) {
        return nullptr;
    }

    return std::move( overlay );
}

int32_t main( int32_t argc, char** argv )
{	
    std::unique_ptr<Overlay> overlay = create_overlay(EDeviceType::Direct3D9, "BLACK DESERT - RANDOM NUMBER");   

    auto* callback = overlay->add_callback(
        "test", []( Surface* surface ) {
            surface->text( 5, 5, "default", 0xFFFF0000, "Press [Insert] To Toggle GUI" );
       }
    );
    if( !callback ) {
        return EXIT_FAILURE;
    }

    while( overlay->render() ) {
		overlay->scale_overlay();
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    overlay->shutdown();
    return EXIT_SUCCESS;
}
