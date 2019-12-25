apt-get update && apt-get install -y apt-transport-https
curl https://mirrors.aliyun.com/kubernetes/apt/doc/apt-key.gpg | apt-key add - 
cat <<EOF >/etc/apt/sources.list.d/kubernetes.list
deb https://mirrors.aliyun.com/kubernetes/apt/ kubernetes-xenial main
EOF  
apt-get update
apt-get install -y kubelet kubeadm kubectl


kubeadm init \
--apiserver-advertise-address=192.168.1.152 \
--image-repository registry.aliyuncs.com/google_containers \
--kubernetes-version v1.14.2 \
--service-cidr=10.1.0.0/16 \
--pod-network-cidr=10.244.0.0/16

echo "export KUBECONFIG=/etc/kubernetes/admin.conf" >> ~/.bash_profile
source ~/.bash_profile

kubeadm join 192.168.1.152:6443 --token i6jdst.aphh189d9kbp4vpa \
    --discovery-token-ca-cert-hash sha256:9df03b7a47ddc636c8773f31b76f8ffff49cee3ca60b728133e5057cbd667c2d

echo "1">/proc/sys/net/ipv4/ip_forward

cat <<EOF >  /etc/sysctl.d/k8s.conf
net.bridge.bridge-nf-call-ip6tables = 1
net.bridge.bridge-nf-call-iptables = 1
EOF
sysctl -p /etc/sysctl.conf


kubeadm init \
--image-repository registry.aliyuncs.com/google_containers \
--kubernetes-version v1.14.2 \

https://raw.githubusercontent.com/kubernetes/dashboard/v1.14.2/src/deploy/recommended/kubernetes-dashboard.yaml