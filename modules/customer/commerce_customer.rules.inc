<?php
// $Id$

/**
 * @file
 * Rules integration for customer profiles.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_customer_rules_event_info() {
  $items = array(
    'commerce_customer_profile_presave' => array(
      'label' => t('Before saving a line item'),
      'group' => t('Commerce'),
      'variables' => commerce_customer_rules_variables('customer_profile'),
      'access callback' => 'commerce_customer_rules_access',
    ),
  );

  // Specify that on presave the customer profile is saved anyway.
  $items['commerce_customer_profile_presave']['variables']['line_item']['skip save'] = TRUE;

  return $items;
}

/**
 * Returns some parameter suitable for using it with a node
 */
function commerce_customer_rules_variables($type) {
  $args = array();

  if ($type == 'customer_profile') {
    $args['customer_profile'] = array(
      'type' => 'commerce_customer_profile',
      'label' => t('Customer profile'),
    );
  }

  return $args;
}

/**
 * Rules integration access callback.
 */
function commerce_customer_rules_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return TRUE;
  }
}

/**
 * @}
 */
