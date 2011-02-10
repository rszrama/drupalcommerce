<?php
// $Id$

/**
 * @file
 * Field handler to present a button to remove a line item. It's a dummy
 * handler, most part of the implementation is done via pre and post render
 * hooks.
 */

/**
 * Field handler to present a button to delete a line item.
 */
class commerce_line_item_handler_field_edit_delete extends commerce_line_item_handler_field_edit {

  function render($values) {
    $line_item_id = $values->{$this->aliases['line_item_id']};

    // Add form that will be used for replacing the placeholder generated bellow
    // at form rendering phase in commerce_line_item_views_post_render().
    $this->form[$line_item_id] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#name' => 'delete-line-item-' . $line_item_id,
      '#attributes' => array('class' => array('delete-line-item')),
    );

    // Return the placeholder that will be used as token for replacing itself
    // with the rendered form field created above.
    return '<!--post-commerce-line-item-edit_delete-' . $line_item_id . '-->';
  }

  function edit_form_submit($form, &$form_state) {
    $order = commerce_order_load($form_state['order']->order_id);

    foreach (element_children($form['edit_delete']) as $line_item_id) {
      // Check for the removal of an item.
      if ($form_state['clicked_button']['#name'] == 'delete-line-item-' . $line_item_id) {
        // Get the title of the line item, if it's a product, use product title
        //  if not, use the line item label
        $line_item = commerce_line_item_load($line_item_id);
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
        if ($line_item_wrapper->type->value() == 'product') {
          $title = $line_item_wrapper->commerce_product->title->value();
        }
        else {
          $title = $line_item_wrapper->line_item_label->value();
        }
        commerce_cart_order_product_line_item_delete($order, $line_item_id);
        drupal_set_message(t('%title removed from your cart.', array('%title' => $title)));
      }
    }
  }

}
