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
  $events = array();

  $events['commerce_customer_profile_presave'] = array(
    'label' => t('Before saving a customer profile'),
    'group' => t('Commerce Customer'),
    'variables' => commerce_customer_rules_event_variables(t('Customer profile'), TRUE),
    'access callback' => 'commerce_customer_rules_access',
  );
  $events['commerce_customer_profile_insert'] = array(
    'label' => t('After saving a new customer profile'),
    'group' => t('Commerce Customer'),
    'variables' => commerce_customer_rules_event_variables(t('Created customer profile'), TRUE),
    'access callback' => 'commerce_customer_rules_access',
  );
  $events['commerce_customer_profile_update'] = array(
    'label' => t('After updating an existing customer profile'),
    'group' => t('Commerce Customer'),
    'variables' => commerce_customer_rules_event_variables(t('Updated customer profile'), TRUE),
    'access callback' => 'commerce_customer_rules_access',
  );
  $events['commerce_customer_profile_delete'] = array(
    'label' => t('After deleting a customer profile'),
    'group' => t('Commerce Customer'),
    'variables' => commerce_customer_rules_event_variables(t('Deleted customer profile')),
    'access callback' => 'commerce_customer_rules_access',
  );

  // Specify that on presave the customer profile is saved anyway.
  $events['commerce_customer_profile_presave']['variables']['customer_profile']['skip save'] = TRUE;

  return $events;
}

/**
 * Returns a variables array for customer profile events.
 *
 * @param $label
 *   The label for the primary customer profile variable.
 * @param $unchanged
 *   Boolean indicating whether or not to include the unchanged customer profile.
 */
function commerce_customer_rules_event_variables($label, $unchanged = FALSE) {
  $variables = array(
    'customer_profile' => array(
      'type' => 'commerce_customer_profile',
      'label' => $label,
    ),
  );

  // Include the unchanged customer profile if specified.
  if ($unchanged) {
    $variables['customer_profile_unchanged'] = array(
      'type' => 'commerce_customer_profile',
      'label' => t('Unchanged customer profile'),
      'handler' => 'rules_events_entity_unchanged',
    );
  }

  return $variables;
}

/**
 * Rules access callback: determines access to Customer events and conditions.
 */
function commerce_customer_rules_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    // Currently there are no restrictions.
    return TRUE;
  }
}

/**
 * @}
 */
