// Apple风格极简博客前端逻辑
const postList = document.getElementById('postList');
const postForm = document.getElementById('postForm');
const titleInput = document.getElementById('title');
const contentInput = document.getElementById('content');

function getPosts() {
  return JSON.parse(localStorage.getItem('blog-posts') || '[]');
}

function savePosts(posts) {
  localStorage.setItem('blog-posts', JSON.stringify(posts));
}

function renderPosts() {
  const posts = getPosts();
  postList.innerHTML = '';
  if (posts.length === 0) {
    postList.innerHTML = '<p style="color:#aaa;text-align:center;">暂无文章，快来写一篇吧！</p>';
    return;
  }
  posts.reverse().forEach(post => {
    const div = document.createElement('div');
    div.className = 'post';
    div.innerHTML = `
      <div class="post-title">${escapeHtml(post.title)}</div>
      <div class="post-content">${escapeHtml(post.content)}</div>
      <div class="post-time">${post.time}</div>
    `;
    postList.appendChild(div);
  });
}

function escapeHtml(str) {
  return str.replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;','\'':'&#39;'}[c]));
}

postForm.onsubmit = function(e) {
  e.preventDefault();
  const title = titleInput.value.trim();
  const content = contentInput.value.trim();
  if (!title || !content) return;
  const now = new Date();
  const time = now.toLocaleString();
  const posts = getPosts();
  posts.push({ title, content, time });
  savePosts(posts);
  titleInput.value = '';
  contentInput.value = '';
  renderPosts();
};

window.onload = renderPosts; 