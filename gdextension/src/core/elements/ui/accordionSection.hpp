#ifndef ACORDION_H
#define ACORDION_H
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

// Accordion section class (collapsible part of the accordion)
class AccordionSection : public VBoxContainer {
    GDCLASS(AccordionSection, VBoxContainer);

private:
    Label* title_label = nullptr;       // The title of the section
    Button* toggle_button = nullptr;    // Button to expand/collapse the section
    Control* content_container = nullptr; // The content that expands and collapses
    bool is_expanded = false;           // Track if the section is expanded
protected:
	static void _bind_methods()
	{}
public:
    // Initialization method called when the section is added to the scene
    void _ready() {
        // Create the label for the section title
        title_label = memnew(Label);
        title_label->set_text("Section Title");  // Default title, you can change this later
        add_child(title_label);                  // Add the label to the section

        // Create the toggle button
        toggle_button = memnew(Button);
        toggle_button->set_text("Expand");       // Default button text
        toggle_button->connect("pressed", callable_mp(this, &AccordionSection::toggle_content));
        add_child(toggle_button);                // Add the button to the section

        // Create the container for the collapsible content
        content_container = memnew(VBoxContainer);
        content_container->set_visible(false);   // Initially hidden
        add_child(content_container);            // Add the content container

        // Add some default content to the content container
        Label* content_label = memnew(Label);
        content_label->set_text("This is the content of the section");
        content_container->add_child(content_label);

        Button* content_button = memnew(Button);
        content_button->set_text("Section Button");
        content_container->add_child(content_button);

        // Debug: Show that the section has been created
        UtilityFunctions::print("Accordion Section Created");
    }

    // Method to toggle the content visibility (expand/collapse)
    void toggle_content() {
        is_expanded = !is_expanded;   // Toggle the expanded state

        // Change button text based on state
        toggle_button->set_text(is_expanded ? "Collapse" : "Expand");

        // Show or hide the content container
        content_container->set_visible(is_expanded);

        // Debug: Print the current state (expanded or collapsed)
        UtilityFunctions::print("Accordion Section ", is_expanded ? "Expanded" : "Collapsed");
    }

    // Function to allow setting the title externally
    void set_section_title(const String& title) {
        if (title_label) {
            title_label->set_text(title);  // Set the title of the section
        }
    }

    // Function to allow adding custom content externally
    void add_custom_content(Control* custom_content) {
        if (content_container) {
            content_container->add_child(custom_content);  // Add custom content to the section
        }
    }
};

// Main accordion class
class Accordion : public VBoxContainer {
    GDCLASS(Accordion, VBoxContainer);
protected:
	static void _bind_methods()
	{}
public:
    // Initialization method called when the accordion is added to the scene
    void _ready() {
        // Create a few accordion sections and add them to the accordion
        for (int i = 0; i < 3; ++i) {
            AccordionSection* section = memnew(AccordionSection);
            section->set_section_title("Section " + String::num(i + 1));  // Set section titles dynamically
            add_child(section);  // Add each section to the accordion
        }

        // Debug: Show that the accordion has been created
        UtilityFunctions::print("Accordion Created");
    }
};
#endif