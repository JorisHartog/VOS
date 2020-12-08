# -*- mode: ruby -*-
# vi: set ft=ruby :

$installer = <<-INSTALLER
yum -y install grub2 xorriso
command -v rustc || \
  (curl https://sh.rustup.rs -sSf -o install.sh; sh ./install.sh -y -q)
source $HOME/.cargo/env
rustup target add thumbv7em-none-eabihf
cd /vagrant
make vos.iso
INSTALLER


Vagrant.configure("2") do |config|
  # All Vagrant configuration is done here. The most common configuration
  # options are documented and commented below. For a complete reference,
  # please see the online documentation at vagrantup.com.

  # Every Vagrant virtual environment requires a box to build off of.
  default_url              = "http://mirror.hostnet.nl/vagrant"
  default_archs            = [ "x86_64" ]

  boxes = {
    'centos8' => {
      'name'  => 'centos_8',
    },
  }

  boxes.each do |imgname, params|
    url_root         = params['url_root']         || default_url
    archs            = params['archs']            || default_archs
    name             = params['name']             || imgname
    archs.each do |arch|
      boxname        = params['boxname']          || "#{name}_#{arch}"
      environment    = params['environment']      || boxname
      hostname       = "#{imgname}-#{arch}".tr('_','-')
      config.vm.define boxname.to_sym do |box|
        box.vm.box = "#{imgname}-#{arch}"
        box.vm.box_url = "#{url_root}/#{imgname}-#{arch}"
        box.vm.hostname = "#{hostname}"
        box.vm.provision "shell", inline: $installer
      end
    end
  end
end


