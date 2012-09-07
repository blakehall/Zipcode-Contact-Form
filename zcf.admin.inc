<?php

/**
 * Page callback for admin/settings/zcf
 */
function zcf_admin_listing() {
  // Print a table listing of contact records.
  // Query zcf_info table print id, name, email, phone.
  $zcf = db_select('zcf_info', 'z')
    ->fields('z')
    ->execute();

  while ($result = $zcf->fetchAssoc()) {
    // TODO add phone, view, edit, delete links.
    $row[] = array($result['zcf_info_id'], $result['name'], $result['email']);
  }

  // TODO add a link to create a new contact record.
  $header = array('id', 'name', 'email');
  $table = theme('table', array('header' => $header, 'rows' => $row));
  return $table;
}

/**
 * Page callback for admin/settings/zcf/new
 */
function zcf_admin_create($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#size' => 20,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#size' => 40,
  );
  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#size' => 20
  );
  $form['zipcodes'] = array(
    '#type' => 'textarea',
    '#title' => t('Zipcodes'),
    '#description' => t('Enter a comma seperated list of zipcodes'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function zcf_admin_create_submit($form, &$form_state) {
  // Save our record.
  $values = $form_state['values'];
//  $zipcodes = $values['zipcodes'];
  $zcf_record = db_insert('zcf_info')
    ->fields(array(
      'name' => $values['name'],
      'email' => $values['email'],
      'phone' => $values['phone'],
    ))
    ->execute();

  drupal_set_message('Record Saved');

  // TODO we also need to properly process and save our zipcode data in the zcf table.

}
